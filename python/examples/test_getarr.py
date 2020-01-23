import sys
import neuralgpu as ngpu

spike = ngpu.Create("spike_generator", 4)
spike0 = spike[0:1]
spike1 = spike[2:2]
spike2 = spike[3:3]

spike_time0 = [10.0, 400.0]
spike_height0 = [1.0, 0.5]
spike_time1 = [4.0]
spike_height1 = [2.0]
spike_time2 = [50.0, 20.0, 80.0]
spike_height2 = [0.1, 0.3, 0.2]


# set spike times and height
ngpu.SetStatus(spike0, {"spike_time": spike_time0,
                        "spike_height":spike_height0})

ngpu.SetStatus(spike1, {"spike_time": spike_time1,
                        "spike_height":spike_height1})

ngpu.SetStatus(spike2, {"spike_time": spike_time2,
                        "spike_height":spike_height2})

print(ngpu.GetNeuronStatus(spike0, "spike_time"))
print(ngpu.GetNeuronStatus(spike0, "spike_height"))
print()
print(ngpu.GetNeuronStatus(spike1, "spike_time"))
print(ngpu.GetNeuronStatus(spike1, "spike_height"))
print()
print(ngpu.GetNeuronStatus(spike2, "spike_time"))
print(ngpu.GetNeuronStatus(spike2, "spike_height"))

print()
print()
neuron_list = [spike[2], spike[3], spike[0], spike[1]]
print(ngpu.GetNeuronStatus(neuron_list, "spike_time"))
print(ngpu.GetNeuronStatus(neuron_list, "spike_height"))



      
      