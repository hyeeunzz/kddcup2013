import sys

if (len(sys.argv) != 3):
    sys.exit("python error.py output solution")
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
    
    
    line = out.readline()
    tmp = line.split(',')
    curr_author = tmp[0]
    s_set = s_dic[curr_author]
    cnt = 0.0
    if int(tmp[-1]) == 1:
        if tmp[1] in s_set:
            cnt+=1
    else:
        if not tmp[1] in s_set:
            cnt+=1
    
    total = 1.0
    errs = []
    for line in out:
        tmp = line.split(',')
        if curr_author != tmp[0]:
            errs.append(cnt/total)
            
            cnt = 0.0
            total = 0.0
            curr_author = tmp[0]
            s_set = s_dic[curr_author]
        
        if int(tmp[-1]) == 1:
            if tmp[1] in s_set:
                cnt+=1
        else:
            if not tmp[1] in s_set:
                cnt+=1
        total +=1
    errs.append(cnt/total)
    
    print 'Error:',1-sum(errs)/len(errs)
    
    out.close()
    sol.close()