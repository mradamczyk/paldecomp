import sys, getopt
import networkx as nx
import graphviz as gv

def dnaComplementarity(x):
    return {
            'A': 'T',
            'C': 'G',
            'G': 'C',
            'T': 'A'
            }.get(x, x)

def check(f, word, i, j, delta, positions):
    if delta < 0:
        return None
    if i > j:
        deletion, swap = False, False
        return [(word[l], any(p == l for p in positions), any(p == l+len(word) for p in positions))
                for l in xrange(len(word))]
    end = None
    if word[i] == f(word[j]):
        end = check(f, word, i+1, j-1, delta, positions)
    if end is not None:
        return end
    end = check(f, word, i+1, j, delta-1, positions + [i])
    if end is not None:
        return end
    end = check(f, word, i, j-1, delta-1, positions + [j])
    if end is not None:
        return end
    end = check(f, word, i+1, j-1, delta-1, positions + [i+len(word), j+len(word)])
    return end

def word2nodes(f, word, delta):
    letters = check(f, word, 0, len(word)-1, delta, [])
    nodes = [('#', False, [])]
    last = nodes[-1]
    for (let, deletion, swap) in letters:
        if deletion:
            last[2].append(let)
        else:
            last = (let, swap, []) 
            nodes.append(last)
    return nodes

def decomposition2graph(f, decomposition, delta):
    g = nx.Graph()
    words = decomposition.split(' ')
    cnt = 1
    g.add_node(0, label='#', pos=[0, 0], color="white")
    last_x, last_y = 0, 0
    for w in words:
        if w[0] == '[':
            last_x += (int(len(w)/5))
            g.add_node(cnt, label=w[1:-1], pos=[last_x, last_y], shape='box', style='bold', color='gray')
            g.add_edge(cnt-1, cnt, style='solid')
            cnt += 1
            last_x += 1 + (int(len(w)/5))
        else:
            nodes = word2nodes(f, w, delta)

            # deletions at the beginning of the word
            if len(nodes[0][2]) == 0:
                last_x += 1
            elif len(nodes[0][2]) == 1:
                last_x += 1
                last_y -= 2
                g.add_node(cnt, label=nodes[0][2][0], pos=[last_x, last_y], color='red')
                g.add_edge(cnt-1, cnt, style='dashed')
                cnt += 1
                last_y += 2
            else:
                last_x += 0
                last_y -= 1
                k = (len(nodes[0][2]) + 1) / 2
                for i in xrange(k):
                    g.add_node(cnt, label=nodes[0][2][i], pos=[last_x, last_y], color='red')
                    g.add_edge(cnt-1, cnt, style='dashed')
                    last_y -= 1
                    cnt += 1

                last_x += 1
                if len(nodes[0][2]) % 2:
                    last_y += 1

                for i in xrange(k, len(nodes[0][2])):
                    g.add_node(cnt, label=nodes[0][2][i], pos=[last_x, last_y], color='red')
                    g.add_edge(cnt-1, cnt, style='dashed')
                    last_y += 1
                    cnt += 1

                last_x += 1

            s = nodes[1:]
            k = len(s) / 2

            temp = []
            for i in xrange(k):
                (let1, swap1, deletions1) = s[i]
                (let2, swap2, deletions2) = s[i+k]

                temp.append((cnt, last_y))
                mx = max(1, max(len(deletions1), len(deletions2)))

                g.add_node(cnt, label=let1, pos=[last_x, last_y], color='blue' if swap1 else 'black')
                g.add_edge(cnt-1, cnt, style='solid')
                cnt += 1

                initial_y = last_y
                if deletions1:
                    last_x += 1 if i == k-1 else -1
                    last_y += 1
                    for let in deletions1:
                        g.add_node(cnt, label=let, pos=[last_x, last_y], color='red')
                        g.add_edge(cnt-1, cnt, style='solid')
                        last_y += 2
                        cnt += 1
                    last_y += 1
                    last_x -= 1 if i == k-1 else -1
                last_y = initial_y + mx * 2

            last_x += 2

            for i in xrange(k-1):
                (let2, swap2, deletions2) = s[i+k]
                (r_id, r_y) = temp.pop()
                g.add_node(cnt, label=let2, pos=[last_x, r_y], color='blue' if swap2 else 'black')
                g.add_edge(cnt-1, cnt, style='solid')
                if r_id != cnt-1:
                    g.add_edge(r_id, cnt, style='dotted')
                cnt += 1

                last_y = r_y
                if deletions2:
                    last_x += 1
                    last_y -= 1
                    for let in deletions2:
                        g.add_node(cnt, label=let, pos=[last_x, last_y], color='red')
                        g.add_edge(cnt-1, cnt, style='solid')
                        last_y -= 2
                        cnt += 1
                    last_y -= 1
                    last_x += 1 if i == k-1 else -1

            (let2, swap2, deletions2) = nodes[-1]
            (r_id, r_y) = temp.pop()
            g.add_node(cnt, label=let2, pos=[last_x, r_y], color='blue' if swap2 else 'black')
            g.add_edge(cnt-1, cnt, style='solid')
            g.add_edge(r_id, cnt, style='dotted')
            cnt += 1

            last_y = r_y
            # deletions at the end of the word
            if len(nodes[-1][2]) == 0:
                last_x += 2
            elif len(nodes[-1][2]) == 1:
                last_x += 1
                last_y -= 1
                g.add_node(cnt, label=nodes[-1][2][0], pos=[last_x, last_y], color='red')
                g.add_edge(cnt-1, cnt, style='dashed')
                cnt += 1
                last_x += 1
                last_y += 1
            else:
                last_x += 0
                last_y -= 1
                k = (len(nodes[-1][2]) + 1) / 2
                for i in xrange(k):
                    g.add_node(cnt, label=nodes[-1][2][i], pos=[last_x, last_y], color='red')
                    g.add_edge(cnt-1, cnt, style='dashed')
                    last_y -= 1
                    cnt += 1

                last_x += 1
                if len(nodes[-1][2]) % 2:
                    last_y += 1

                for i in xrange(k, len(nodes[-1][2])):
                    g.add_node(cnt, label=nodes[-1][2][i], pos=[last_x, last_y], color='red')
                    g.add_edge(cnt-1, cnt, style='dashed')
                    last_y += 1
                    cnt += 1

                last_x += 1
            last_x += 1
    return g

def drawGraph(g):
    G = gv.Graph(engine='neato', format='svg')
    G.graph_attr['size'] = '10'
    for node, values in g.nodes(data=True):
        (x, y) = values["pos"]
        G.node("%d" % node, values["label"], fontsize='40', style='bold', bgcolor=values.get('fillcolor', 'white'), color=values.get('color', 'black'), pos="%d,%d!" % (x, y), shape=values.get('shape', 'circle'))
    for (x, y, values) in g.edges(data=True):
        G.edge("%d" % x, "%d" % y, style=values["style"])
    G.render("decomposition")

def main(argv):
    dna, delta = False,  0
    try:
        opts, args = getopt.getopt(argv,"dE:")
    except getopt.GetoptError:
        print ('drawDecomposition.py [-d] [-E N]')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-d':
            dna = True
        elif opt == '-E':
            delta = int(arg)

    sys.stdin.readline()
    decomp = sys.stdin.readline().strip()
    g = decomposition2graph(dnaComplementarity if dna else (lambda x: x), decomp, delta)
    drawGraph(g)

if __name__ == "__main__":
    main(sys.argv[1:])

