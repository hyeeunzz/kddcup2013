import sys
import re

def postprocessing(critical_dict, author_id, paper_ids, flag):
    if flag == 0:
        return paper_ids

    head = []
    tail = []
    for paper_id in paper_ids:
        key = author_id + ',' + paper_id
        value = critical_dict[key]
        if (paper_id in head) or value < 2:
            tail.append(paper_id)
        else:
            head.append(paper_id)
    return head + tail


if (len(sys.argv) != 3):
    sys.exit("python eval.py input {0/1} # 0/1: use critical feature or not")
else:
    critical_flag = int(sys.argv[2])
    # read critical feature
    critical_dict = {}
    if critical_flag == 1:
        datadir = '/'.join(re.split('\\\\|/', sys.argv[1])[:-1])
        criticalFeaturePath = datadir + '/' + re.split('\\\\|/', sys.argv[1])[-1].split('.')[0] + '.feature.301.csv'
        critical_f = open(criticalFeaturePath, 'r')
        critical_f.readline()
    
        for line in critical_f:
            author_id = line.split(',')[0]
            paper_id  = line.split(',')[1]
            feature   = line.split(',')[2]
            critical_dict[author_id + ',' + paper_id] = float(feature)

    # start

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
            paper = " ".join(postprocessing(critical_dict, curr_author, front_p+mid_p+rear_p, critical_flag))+"\n"
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
    paper = " ".join(postprocessing(critical_dict, curr_author, front_p+mid_p+rear_p, critical_flag))+"\n"
    out_f.write(author+paper)
    
    in_f.close()
    out_f.close()
    