import sys

if (len(sys.argv) != 3):
    sys.exit("python eval.py output solution")
else:
    outputPath = './' + sys.argv[1]
    solutionPath = './' + sys.argv[2]
    
    out = open(outputPath,'r')
    sol = open(solutionPath,'r')
    out.readline()
    sol.readline()
    
    s_dic = {}
    
    for one_line in sol:
        line = one_line.split(',')
        line[1] = line[1].split()
        s_dic[line[0]] = line[1]

    maps = []
    for one_line in out:
        line = one_line.split(',')
        line[1] = line[1].split()
        sol_list = s_dic[line[0]]
        cnt = 0.0
        t_sum = 0.0
        for i in range(len(line[1])):
            if line[1][i] in sol_list:
                cnt+=1
                t_sum += cnt/(i+1)
                sol_list.remove(line[1][i])
        maps.append(t_sum/cnt)
    
    print 'MAP:',sum(maps)/len(maps)
    
    out.close()
    sol.close()