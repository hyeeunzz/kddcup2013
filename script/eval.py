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
    
    for line in sol:
        s_key, s_papers = line.split(',')[:2]
        s_set = set(s_papers.split())
        s_dic[s_key] = s_set

    maps = []
    for one_line in out:
        key, papers = one_line.split(',')[:2]
        p_list = papers.split()
        sol_set = s_dic[key]
        cnt = 0.0
        t_sum = 0.0
        for i in range(len(p_list)):
            if p_list[i] in sol_set:
                cnt+=1
                t_sum += cnt/(i+1)
                sol_set.remove(p_list[i])
        
        maps.append(t_sum/cnt)
    
    print 'MAP:',sum(maps)/len(maps)
    
    out.close()
    sol.close()