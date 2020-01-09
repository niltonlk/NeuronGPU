/*
Copyright (C) 2020 Bruno Golosio
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "neuralgpu.h"

using namespace std;

int main(int argc, char *argv[])
{
  // Intializes C random number generator
  // srand((unsigned) time(&t));

  NeuralGPU ngpu;
  cout << "Building ...\n";

  ngpu.SetMaxSpikeBufferSize(10); //max buffer size per neuron
  
  // poisson generator parameters
  float poiss_rate = 5000.0; // poisson signal rate in Hz
  float poiss_weight = 1.0;
  float poiss_delay = 0.2; // poisson signal delay in ms
  int n_pg = 7; // number of poisson generators
  // create poisson generator
  int pg = ngpu.CreatePoissonGenerator(n_pg, poiss_rate);

  // create 3 neuron groups
  int n_neur1 = 100; // number of neurons
  int n_recept1 = 3; // number of receptors
  int neur_group1 = ngpu.CreateNeuron("AEIF", n_neur1, n_recept1);
  int n_neur2 = 20; // number of neurons
  int n_recept2 = 1; // number of receptors
  int neur_group2 = ngpu.CreateNeuron("AEIF", n_neur2, n_recept2);
  int n_neur3 = 50; // number of neurons
  int n_recept3 = 2; // number of receptors
  int neur_group3 = ngpu.CreateNeuron("AEIF", n_neur3, n_recept3);
  
  // the following parameters are set to the same values on all hosts
  float E_rev[] = {0.0, 0.0, 0.0};
  float taus_decay[] = {1.0, 1.0, 1.0};
  float taus_rise[] = {1.0, 1.0, 1.0};
  ngpu.SetNeuronVectParams("E_rev", neur_group1, n_neur1, E_rev, 3);
  ngpu.SetNeuronVectParams("taus_decay", neur_group1, n_neur1,
				 taus_decay, 3);
  ngpu.SetNeuronVectParams("taus_rise", neur_group1, n_neur1,
				 taus_rise, 3);
  ngpu.SetNeuronVectParams("E_rev", neur_group2, n_neur2, E_rev, 1);
  ngpu.SetNeuronVectParams("taus_decay", neur_group2, n_neur2,
				 taus_decay, 1);
  ngpu.SetNeuronVectParams("taus_rise", neur_group2, n_neur2,
				 taus_rise, 1);
  ngpu.SetNeuronVectParams("E_rev", neur_group3, n_neur3, E_rev, 2);
  ngpu.SetNeuronVectParams("taus_decay", neur_group3, n_neur3,
				 taus_decay, 2);
  ngpu.SetNeuronVectParams("taus_rise", neur_group3, n_neur3,
				 taus_rise, 2);

  int i11 = neur_group1 + rand()%n_neur1;
  int i12 = neur_group2 + rand()%n_neur2;
  int i13 = neur_group2 + rand()%n_neur2;
  int i14 = neur_group3 + rand()%n_neur3;

  int i21 = neur_group2 + rand()%n_neur2;

  int i31 = neur_group1 + rand()%n_neur1;
  int i32 = neur_group3 + rand()%n_neur3;

  int it1 = neur_group1 + rand()%n_neur1;
  int it2 = neur_group2 + rand()%n_neur2;
  int it3 = neur_group3 + rand()%n_neur3;
  
  // connect poisson generator to port 0 of all neurons
  ngpu.Connect(pg, i11, 0, poiss_weight, poiss_delay);
  ngpu.Connect(pg+1, i12, 0, poiss_weight, poiss_delay);
  ngpu.Connect(pg+2, i13, 0, poiss_weight, poiss_delay);
  ngpu.Connect(pg+3, i14, 0, poiss_weight, poiss_delay);
  ngpu.Connect(pg+4, i21, 0, poiss_weight, poiss_delay);
  ngpu.Connect(pg+5, i31, 0, poiss_weight, poiss_delay);
  ngpu.Connect(pg+6, i32, 0, poiss_weight, poiss_delay);

  float weight = 0.01; // connection weight
  float delay = 0.2; // connection delay in ms

  // connect neurons to target neuron n. 1
  ngpu.Connect(i11, it1, 0, weight, delay);
  ngpu.Connect(i12, it1, 1, weight, delay);
  ngpu.Connect(i13, it1, 1, weight, delay);
  ngpu.Connect(i14, it1, 2, weight, delay);

  // connect neuron to target neuron n. 2
  ngpu.Connect(i21, it2, 0, weight, delay);

    // connect neurons to target neuron n. 3
  ngpu.Connect(i31, it3, 0, weight, delay);
  ngpu.Connect(i32, it3, 1, weight, delay);
  
  // create multimeter record n.1
  string filename1 = "test_neuron_groups_voltage.dat";
  int i_neuron_arr1[] = {i11, i12, i13, i14, i21, i31, i32, it1, it2, it3};
  string var_name_arr1[] = {"V_m", "V_m", "V_m", "V_m", "V_m", "V_m",
			    "V_m", "V_m", "V_m", "V_m"};
  int record1 = ngpu.CreateRecord(filename1, var_name_arr1,
				  i_neuron_arr1, 10);

  // create multimeter record n.2
  string filename2 = "test_neuron_groups_g1.dat";
  int i_neuron_arr2[] = {it1, it1, it1, it2, it3, it3};
  int i_receptor_arr[] = {0, 1, 2, 0, 0, 1};
  string var_name_arr2[] = {"g1", "g1", "g1", "g1", "g1", "g1"};
  //int record2 =
  ngpu.CreateRecord(filename2, var_name_arr2,
		    i_neuron_arr2, i_receptor_arr, 6);

  // create multimeter record n.3
  string filename3 = "test_neuron_groups_spikes.dat";
  int i_neuron_arr3[] = {i11, i12, i13, i14, i21, i31, i32};
  string var_name_arr3[] = {"spike", "spike", "spike", "spike", "spike",
				 "spike", "spike"};
  //int record3 =
  ngpu.CreateRecord(filename3, var_name_arr3,
		    i_neuron_arr3, 7);

  ngpu.SetRandomSeed(1234ULL);
  ngpu.Simulate();

  std::vector<std::vector<float>> data_vect1 =
    *ngpu.GetRecordData(record1);

  FILE *fp=fopen("test_neuron_group_record.dat", "w");
  for (uint i=0; i<data_vect1.size(); i++) {
    std::vector<float> vect = data_vect1[i];
    for (uint j=0; j<vect.size()-1; j++) {
      fprintf(fp,"%f\t", vect[j]);
    }
    fprintf(fp,"%f\n", vect[vect.size()-1]);
  }
  fclose(fp);
  
  return 0;
}