import mymodules as h
import numpy as np
import matplotlib.pyplot as plt
#from analysis import getFeatures
from analysisWithPlotting import getFeatures
#parameters are contained in individual
# so par1 is individual[0]
# and par2 is individual[1]
def evaluate(individual, col, lw, period, GMI, GCAS):
	

	GL = individual[0]
	GH = individual[1]
	GCa = individual[2]
	GKCa = individual[3]
	GA = individual[4]



	lp = h.Neuron.create("lp")
	soma = h.Compartment.create("soma", -65.0)
	soma.initialize(1.0/3.0, 40.0)
	gleak = h.leak.create(GL, -50)
	soma.add_conductance(gleak)
	gh = h.H.create(GH, -10)
	soma.add_conductance(gh)
	gCaOhmic = h.CaOhmic.create(GCa, 120.0)
	soma.add_conductance(gCaOhmic)
	gKCa = h.KCa.create(GKCa, -80.0)
	soma.add_conductance(gKCa)
	gA = h.Ka.create(GA, -80.0)
	soma.add_conductance(gA)
	gmi = h.Mi.create(0, -10.0)
	soma.add_conductance(gmi)
	gCaS = h.CaS.create(0, 120)
	soma.add_conductance(gCaS)
	CaInt = h.CaInt.create(6.49)
	soma.add_sensor(CaInt)

	#gSynAB = h.SynAB.create(1000, 6.0, -70.0)
	#soma.add_conductance(gSynAB)
	#gSynPD = h.SynPD.create(1000, 5.0, -70.0)
	#soma.add_conductance(gSynPD)
	#gSynPY = h.SynPY.create(1000, 1, -80.0)
	#soma.add_conductance(gSynPY)

	dclampGSyn = h.dclampSyn.create(6.0, -80)
	soma.add_conductance(dclampGSyn)

	# add resistor node
	resistor = h.Compartment.create("resistor", -55.0)
	resistor.initialize(0.0, 0.0)


	# add axon
	axon = h.Compartment.create("axon", -55.0)
	axon.initialize(1.0/3.0, 10.0)
	gleak2 = h.leak.create(2.5, -55.0)
	axon.add_conductance(gleak2)
	gNa = h.NaV.create(750.0, 50.0)
	axon.add_conductance(gNa)
	gKdr = h.Kdr.create(225.0, -80.0)
	axon.add_conductance(gKdr)

	lp.addCompartment(resistor)
	lp.addCompartment(soma)
	lp.addCompartment(axon)

	lp.connect_compartments(soma, resistor)
	lp.connect_compartments(resistor, axon)

	dim = lp.get_state_dim()

	# create initial y vector
	yVec = h.VectorOfDouble()
	yVec[:] = np.zeros(dim)
	lp.init(yVec)

	neurons = h.NeuronVec()
	neurons[:] = [lp]
	model = h.NeuronModel.create(neurons, dim)

	t = h.VectorOfDouble()
	v = h.VectorOfDouble()
	iSyn = h.VectorOfDouble()
	# 0 for variable and 1 for fixed
	model.run(1, 10000, yVec, t, v, iSyn)

	# plot the voltage traces
	#plt.plot(t, v, color="blue", lw=5)
	#plt.show()


	period = 1000
	[ slow_wave_amp, burst_onset_latency, burst_duration, intra_burst_freq, v_base, num_spks] = getFeatures(t, v, iSyn, col, lw)
	if burst_onset_latency == np.inf:
	    burst_onset_phase = np.inf
	else:
	    burst_onset_phase = burst_onset_latency/period

	a = ((slow_wave_amp-33.686484592)/33.686484592)**2
	b = ((burst_onset_phase-0.52577524786)/0.52577524786)**2
	c = ((burst_duration-307.029560151)/307.029560151)**2
	d = ((intra_burst_freq-29.3131384339)/29.3131384339)**2
	e = ((v_base+63.0948501317)/63.0948501317)**2
	f = ((float(num_spks)-9.0)/9.0)**2


	print a, b, c, d, e, f
	
	#return [a, b, c, d, e, f]
	return [slow_wave_amp, burst_onset_latency, burst_duration, intra_burst_freq, v_base, num_spks]
