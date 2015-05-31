import sys

if (len(sys.argv) != 2):
    sys.exit("python eval.py input")
else:
    inputPath = './' + sys.argv[1]
    outputPath = './' + sys.argv[1][:-4]+'.result.csv'
    
    in_f = open(inputPath,'r')
    out_f = open(outputPath,'w')
    in_f.readline()
    out_f.write("AuthorId,PaperIds\n")
    
    front_p = []
    mid_p = []
    rear_p = []    
    
    line = in_f.readline()
    tmp = line.split(',')
    curr_author = tmp[0]
    if int(tmp[-1]) == 1:
        front_p.append(tmp[1])
    else:
        mid_p.append(tmp[1])
    
    for line in in_f:
        tmp = line.split(',')
        
        if curr_author != tmp[0]:
            author = curr_author+","
            paper = " ".join(front_p+mid_p+rear_p)+"\n"
            out_f.write(author+paper)
            
            front_p[:] = []
            mid_p[:] = []
            rear_p[:] = []
            curr_author = tmp[0]
        
        if int(tmp[-1]) == 1:
            if tmp[1] in front_p:
                rear_p.append(tmp[1])
            else:
                front_p.append(tmp[1])
        else:
            if tmp[1] in mid_p:
                rear_p.append(tmp[1])
            else:
                mid_p.append(tmp[1])
    author = curr_author+","
    paper = " ".join(front_p+mid_p+rear_p)+"\n"
    out_f.write(author+paper)
    
    in_f.close()
    out_f.close()
    