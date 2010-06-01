#!/usr/bin/python
import sys
import os
import re

class msg(object):
    def __init__(self, data):
        self.time = int(data[0])
        self.subject = int(data[1].replace("ID:",""))
        self.msg = data[2]
        self._msg_parse()

    def _msg_parse(self):
        pass
    def __repr__(self):
        return "%s, io sono %d, e dico %s"%(self.time, self.subject, self.msg)


class dataMsg(msg):
    
    def _msg_parse(self):
        tmp = re.search("(\d+) .+ (\d+)", self.msg)
        if tmp:
            if self.msg.startswith("DATA send"):
                self.send=True
                self.other_side = int(tmp.group(1)) 
                self.counter = int(tmp.group(2))
            else:
                self.send=False
                self.other_side = int(tmp.group(2)) 
                self.counter = int(tmp.group(1))
        else:
            print "non va la regular expression"
            exit(0)

    def __repr__(self):
        return "%s, io sono %d, e dico %s"%(self.time, self.subject, self.msg)

class rplMsg(msg):
    def _msg_parse(self):
        g = re.search("rank=(\d+),stats=(\d+) (\d+) (\d+) (\d+),", self.msg)
        if g:
            self.rank = g.group(1)
            self.tot_dio = g.group(2) # totail dio
            self.send = g.group(3)
            self.recv = g.group(4)
            self.unknown_2 = g.group(5)  # unknown 
        else:
            print self.msg
            exit(0)

class routeMsg(msg):
    pass

def msgFoundry(data):
    if data[2].startswith("DATA"):
        return dataMsg(data)
    elif data[2].startswith("#A"):
        return rplMsg(data)
    elif data[2].startswith("#L"):
        return routeMsg(data)
    else:
        return None

def parse(l):
    return msgFoundry(l.split("\t",2))

delay = {}
rank_count=0
overhead = []
oh_data = 0
oh_rpl = 0

n_received = 0

def list_sort(a, b):
    if((a[0]-b[0])>0):
        return 1

if __name__=="__main__":
    if sys.argv.__len__() != 2:
        print "devi inserire il nome del file"
        exit(0)
    f_name = sys.argv[1]
    print "scansione del file %s ..."%f_name
    if not os.path.exists(f_name):
        print "Il file non esiste"
        exit(0)
    f = open(f_name)
    for l in f: # il file una linea per volta!!!! ##
        tmp = parse(l)
        if tmp:
            if not delay.has_key(tmp.subject):
                delay[tmp.subject] = {}
            if type(tmp) == dataMsg:   ## DATA
                if tmp.send:
                    try:
                        delay[tmp.subject][tmp.counter] = tmp.time
                    except Exception, e:
                        print e
                    oh_data += 1
                    if( oh_data == 0):
                        overhead.append([tmp.time, oh_data, oh_rpl, 999999999999])
                    else:
                        overhead.append([tmp.time, oh_data, oh_rpl, oh_rpl/oh_data])
                    #print "%s, %d(%s) -> %d"%(tmp.time, tmp.subject, tmp.counter, tmp.other_side)
                else:  # received
                    delay[tmp.other_side][tmp.counter] = tmp.time - delay[tmp.other_side][tmp.counter] 
                    n_received += 1
                    #print "%s, sync riceve %d da %d"%(tmp.time, tmp.counter, tmp.other_side)
                    #### END DATA ####
            elif type(tmp)== rplMsg:   ### RPL ###
                rank_count += 1
                #print "%d, %d"%(tmp.time, rank_count) 
                oh_rpl += 1
                if( oh_data == 0):
                    overhead.append([tmp.time, oh_data, oh_rpl, 999999999999])
                else:
                    overhead.append([tmp.time, oh_data, oh_rpl, oh_rpl/oh_data])

                 #### FINE RPL ###

       ##### finito il file ###
    total_delay = 0
    n_pkt = 0
    for i in delay:
        for j in delay[i]:
            n_pkt += 1
            total_delay += delay[i][j]
    print n_pkt
    print total_delay/n_pkt
    print n_received
    f.close()

