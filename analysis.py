#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
import peakutils 

def getFeatures(x, y, z):
    
    x = np.fromiter(x, dtype = np.float)
    y = np.fromiter(y, dtype = np.float)
    z = np.fromiter(z, dtype = np.float)

    #print "hello at beginning"
    
    indices = peakutils.indexes(z, thres = 0.75, min_dist=100)
    t = x[indices[-3]:indices[-2]]
    v = y[indices[-3]:indices[-2]]
    z = z[indices[-3]:indices[-2]]
   
   
    indices2 = peakutils.indexes(v, thres = 0.001, min_dist=1)
   
    #plt.plot(t, v, color=col, lw=thickness)
	

    slow_wave_amp = np.amax(v) - np.amin(v)
    burst_onset_latency = 0
    burst_duration = 0
    intra_burst_freq = 0
    num_spks = 0
    v_base = np.amin(v)
    flag = 0

    if len(indices2)> 1:

	
	v_negative =  np.multiply(v, -1.0)
        indices_minima = peakutils.indexes(v_negative, thres = 0.001, min_dist=1)
	#plt.plot(t[indices2], v[indices2], 'ok', t[indices_minima], v[indices_minima], 'ok')

	real_Spike_inds = []
	#print len(indices_minima), len(indices2)
	indices_minima=indices_minima[0:len(indices2)]
	if len(indices_minima) == len(indices2)-1:
	    indices_minima = np.append(indices_minima,len(v)-1)
 	
	flag = 0

	#plt.plot(t[indices2], v[indices2], 'og', markersize=20)
	#plt.plot(t[indices_minima], v[indices_minima], 'oy', markersize=20)

	deleteMinima = []
        #print len(indices2), len(indices_minima)
	if len(indices2)> 1:
	    if t[indices2[0]] > t[indices_minima[0]]:
		
		indices_minima = np.delete(indices_minima, 0)
		#print len(indices2), len(indices_minima)
	        if len(indices2) != len(indices_minima):
		    indices_minima=np.append(indices_minima, len(v)-1)
		    #print len(indices2), len(indices_minima)


		#print len(indices_minima), len(indices2)
	    
	    for (i, tt) in enumerate(t[indices2]):
		
		flagmin = 0
		flag = 0
		#print i
		#print abs(t[indices2[i]] - t[indices_minima[i]]), v[indices2[i]] - v[indices_minima[i-1]]

	    	if (t[indices_minima[i]] - t[indices2[i]]) > 60:
		    #print "hello1"
	            flag = 1

            	elif v[indices2[i]] - v[indices_minima[i]] < 2:
		    #print "hello2"
	            flag = 1
		

		if flag == 1:
	    	    real_Spike_inds.append(i)

	#print len(indices2), len(indices_minima)
        indices2 = np.delete(indices2, real_Spike_inds)
	indices_minima = np.delete(indices_minima, real_Spike_inds)
	#print len(indices2), len(indices_minima)
	#plt.plot(t[indices2], v[indices2], 'or', markersize=20)
	#plt.plot(t[indices_minima], v[indices_minima], 'or', markersize=20)
	#plt.show()

	# condition to check if all maxima and minima have been deleted...if so, then there are no spikes
        if (len(indices2)==0):
            slow_wave_amp = np.amax(v) - np.amin(v)
            burst_onset_latency = 0
            burst_duration = 0
            intra_burst_freq = 0
            num_spks = 0
	    v_base = np.amin(v)

    	else:
	    if len(indices_minima) == len(indices2)-1:
	        indices_minima = np.append(indices_minima,len(v)-1)
 	        #print len(indices2), len(indices_minima)
	
      



	    # extract bursts from event times
	    t_events = t[indices2]
	    index_first_event_of_this_burst=0
	    burst_start_inds=[]
	    burst_end_inds=[]
	    t_burst_starts=[]
	    t_burst_ends=[]
    
        


	
            for j in range(len(indices2)-1):
            #print j 
                if (t_events[j+1] - t_events[j]) > 120:
                    t_burst_starts.append(t_events[index_first_event_of_this_burst])
                    burst_start_inds.append(indices2[index_first_event_of_this_burst])
                    t_burst_ends.append(t[i])
                    burst_end_inds.append(i)
                    index_first_event_of_this_burst=j+1

                    #print j

     
     
            t_burst_starts.append(t_events[index_first_event_of_this_burst])
            t_burst_ends.append(t_events[-1])
            burst_start_inds.append(indices2[index_first_event_of_this_burst])
            burst_end_inds.append(indices2[-1])

	
    
	
            # sub condition= if there are multiple sub bursts within a burst give this bad scores for features
            if (len(t_burst_starts) == 1):
            	#print "HELLLLLOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!!!"
                slow_wave_amp = np.amax(v[indices_minima])-np.amin(v)
                burst_onset_latency = t_burst_ends[0]-x[indices[-3]]
	        if (len(indices2) > 1):
                    intra_burst_freq = 1000*((len(indices2))/(t_burst_ends[0]-t_burst_starts[0]))
	    	else:
		    intra_burst_freq = 0

                burst_duration = t_burst_ends[0] - t_burst_starts[0]
                num_spks = len(indices2)
	        #print "The burst duration is ", t_burst_ends[0]-t_burst_starts[0]
	        #print "Number of spikes in burst is ", len(indices2)
	        #print "burst latency is ", t_burst_starts[0] - x[indices[-2]]
	        #print "The intraburst spike frequency is ", 1000*(np.mean(t[indices2[1:]]-t[indices2[0:-1]])/(t_burst_ends[0]-t_burst_starts[0])), 'Hz'

            else:
                #print "HELLLLLOOOOOOOOOOOOOOOOOOOOOO"
                slow_wave_amp = np.amax(v[indices_minima])-np.amin(v)
            	burst_onset_latency = t_burst_starts[0]-x[indices[-3]]
            	burst_duration = 0
            	intra_burst_freq = 0
            	num_spks = len(indices2)
            	#print "The burst duration is ", np.inf
            	#print "Number of spikes in burst is ", len(indices2)
            	#print "burst latency is ", burst_onset_latency
            	#print "The intraburst spike frequency is ", np.inf, 'Hz'
    

            v_base = np.amin(v)
        #print "The slow-wave amplitude is ", slow_wave_amp, " mV"
        #print "voltage base is ", np.amin(v)
        #print "The average spike height is ", 
            vmin_Idx = np.argmin(v)


        #print slow_wave_amp, burst_onset_latency, burst_duration, intra_burst_freq, v_base, num_spks

    return [ slow_wave_amp, burst_onset_latency, burst_duration, intra_burst_freq, v_base, num_spks] 


    #return [1, 1,1,1,1,1]
