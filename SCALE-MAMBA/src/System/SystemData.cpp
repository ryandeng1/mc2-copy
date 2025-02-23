/*
Copyright (c) 2017, The University of Bristol, Senate House, Tyndall Avenue, Bristol, BS8 1TH, United Kingdom.
Copyright (c) 2018, COSIC-KU Leuven, Kasteelpark Arenberg 10, bus 2452, B-3001 Leuven-Heverlee, Belgium.

All rights reserved
*/

#include "SystemData.h"
#include "Exceptions/Exceptions.h"
#include <fstream>

#include <iostream>
SystemData::SystemData(const string &NetworkDataFileName)
{
  unsigned int i, j;
  ifstream inp(NetworkDataFileName.c_str());
  if (inp.fail())
    {
      throw file_error(NetworkDataFileName.c_str());
    }

  unsigned int numplayers;
  string RootCertName;
  vector<string> IP_Numbers, PlayerCertFiles, PlayerNames;
  int fake_off, fake_sac;

  inp >> RootCertName;
  inp >> numplayers;

  IP_Numbers.resize(numplayers);
  PlayerCertFiles.resize(numplayers);
  PlayerNames.resize(numplayers);
  for (i= 0; i < numplayers; i++)
    {
      inp >> j >> IP_Numbers[i] >> PlayerCertFiles[i] >> PlayerNames[i];
      cout << j << endl;
      cout << IP_Numbers[i] << endl;
      cout << PlayerCertFiles[i] << endl;
      cout << PlayerNames[i] << endl;
    }

  inp >> fake_off;
  inp >> fake_sac;

  fake_off = 1;
  fake_sac = 1;
  semihonest = 1;
#ifndef SH 
  semihonest = 0;
#endif

  init(numplayers, RootCertName, IP_Numbers, PlayerCertFiles, PlayerNames, fake_off, fake_sac, semihonest);
}

void SystemData::init(unsigned int numplayers, const string &RootCertName,
                      const vector<string> &IP_Numbers,
                      const vector<string> &PlayerCertFiles,
                      const vector<string> &PlayerNames,
                      int fake_off, int fake_sac, int semihonest_param)
{
  n= numplayers;
  RootCRT= RootCertName;
  IP.resize(n);
  PlayerCRT.resize(n);
  PlayerCN.resize(n);
  if (n != IP_Numbers.size() || n != PlayerCertFiles.size() || n != PlayerNames.size())
    {
      throw data_mismatch();
    }
  IP= IP_Numbers;
  PlayerCRT= PlayerCertFiles;
  PlayerCN= PlayerNames;
  fake_offline= 1;
  fake_sacrifice= 1;
  semihonest = semihonest_param;
}
