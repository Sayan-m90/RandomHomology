//
//  SimPersWrapper.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/10/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "SimPersWrapper.hpp"

extern int complexSize;
extern int iThreshold;
extern vector<float> vecFiltrationScale;

extern std::clock_t start, timer1;
extern double dFuncTimeSum;
extern double dInsertTime;
extern double dCollapseTime;

void ComputingPersistenceForSimplicialMap(const char* file_name_of_domain_complex,
                                          bool is_domain_complex_with_annotation,
                                          const char* file_name_of_range_complex,
                                          const char* file_name_of_simplicial_map,
                                          bool is_save_range_complex_with_annotation = false,
                                          const char* new_range_complex_file_name = NULL);

void ComputingPersistenceForSimplicialMapElementary(const char* file_name_of_domain_complex,
                                                    bool is_domain_complex_with_annotation,
                                                    vector<string>& vecElemOpers,
                                                    bool is_save_range_complex_with_annotation = false,
                                                    const char* new_range_complex_file_name = NULL);

void SimpersWrapper::Run(vector<Operation *> c,
                         //vector<Barcode> &output,
                         string fp) {
    
    // eventually use operations directly instead of writing to file
    cout << "Running SimPers" << endl;
    
    start = std::clock();
    string input_domain_complex_file_name = fp + "_iDC";
    // input range??
    string input_range_complex_file_folder = "/";
    string input_simplicial_map_file = fp + "_collapses";
    string output_range_complex_with_annotation_file_name = "/Users/billvarcho/Desktop/Tests/out_file_1";
    string output_persistence_file_name = "/Users/billvarcho/Desktop/Tests/out_file_2";
    bool is_input_domain_complex_with_annotation = false;
    bool is_output_range_complex_with_annotation = false;
    bool is_elementary = true;
    
    
    // TODO(me): dont hardcode lol
    iThreshold = 0;
    int numFiltration = 57;
    start = std::clock();

    string sDelimiter("#");
    vecFiltrationScale.push_back(0);
    float fMaxScale = 0.0;
    if (!is_elementary)
    {
        if (input_range_complex_file_folder.empty())
        {
            std::cout << "In general simplicial map mode, parameter -r is required." << endl;
            return;
        }
    }
    ifstream ifs_m;
    for (int i = 1; i <= numFiltration; i++)
    {
        if (!is_elementary)
        {
            string sRC = input_range_complex_file_folder;
            string sMap = input_simplicial_map_file;
            stringstream ss;
            ss << i;
            sRC += "/";
            sRC += ss.str();
            sRC += ".txt";
            sMap += "/";
            sMap += ss.str();
            sMap += ".txt";
            //timer
            timer1 = std::clock();
            if (i == numFiltration)
            {
                filtration_step = i;
                ComputingPersistenceForSimplicialMap(input_domain_complex_file_name.c_str(), is_input_domain_complex_with_annotation,
                                                     sRC.c_str(), sMap.c_str(), is_output_range_complex_with_annotation,
                                                     output_range_complex_with_annotation_file_name.c_str());
            }
            else
            {
                if (i == 1)
                    filtration_step = 0;
                else
                    filtration_step = i;
                ComputingPersistenceForSimplicialMap(input_domain_complex_file_name.c_str(), is_input_domain_complex_with_annotation,
                                                     sRC.c_str(), sMap.c_str(), false,
                                                     output_range_complex_with_annotation_file_name.c_str());
            }
            dFuncTimeSum += (std::clock() - timer1) / (double)CLOCKS_PER_SEC;
        }
        else
        {
            if (i == 1)
            {
                ifs_m.open(input_simplicial_map_file);
                if (!ifs_m.is_open())
                {
                    std::cout << "can't open file!" << endl;
                    return;
                }
            }
            vector<string> vecOpers;
            string sOper;
            char sLine[256];
            while (ifs_m.getline(sLine, 256))
            {
                if (sLine[0] == '#')
                {
                    stringstream ss;
                    ss.str(sLine);
                    string sSharp;
                    float fScale;
                    ss >> sSharp;
                    ss >> fScale;
                    vecFiltrationScale.push_back(fScale);
                    break;
                }
                sOper = sLine;
                vecOpers.push_back(sOper);
            }
            //timer
            timer1 = std::clock();
            if (i == numFiltration)
            {
                filtration_step = i;
                ComputingPersistenceForSimplicialMapElementary("", false,
                                                               vecOpers, is_output_range_complex_with_annotation,
                                                               output_range_complex_with_annotation_file_name.c_str());
            }
            else
            {
                if (i == 1)
                    filtration_step = 0;
                else
                    filtration_step = i;
                ComputingPersistenceForSimplicialMapElementary(input_domain_complex_file_name.c_str(), is_input_domain_complex_with_annotation,
                                                               vecOpers, false,
                                                               output_range_complex_with_annotation_file_name.c_str());
            }
            dFuncTimeSum += (std::clock() - timer1) / (double)CLOCKS_PER_SEC;
        }
    }
    ifs_m.close();
    ifs_m.clear();

    
    ofstream ofs(output_persistence_file_name);
    for (int i = 0; i < persistences.size(); ++i)
    {
        ofs << "Dim " << i << ": " << endl;
        std::unordered_map<int, pair<int, int>>::iterator it;
        for (it = persistences[i].begin(); it != persistences[i].end(); ++it)
            
        {
            ofs << vecFiltrationScale[(it->second).first] << " ";
            if ((it->second).second == -1)
                ofs << "inf" << endl;
            else
                ofs << vecFiltrationScale[(it->second).second] << endl;
        }
        ofs << endl;
    }
    ofs.close();
    ofs.clear();
    for (int k = 0; k < vecFiltrationScale.size(); ++k)
    {
        if (vecFiltrationScale[k] > fMaxScale)
            fMaxScale = vecFiltrationScale[k];
    }
    
    //output timing result
    dInsertTime = dInsertTime / (double)CLOCKS_PER_SEC;
    dCollapseTime = dCollapseTime / (double)CLOCKS_PER_SEC;
    dFuncTimeSum = dInsertTime + dCollapseTime;
    ofstream ofsTime("Timing.txt");
    ofsTime << "Total time: " << (std::clock() - start) / (double)CLOCKS_PER_SEC << "s" << endl;
    ofsTime << "Operation time: " << dFuncTimeSum << "s" << endl;
    ofsTime << "Insert time: " << dInsertTime << "s" << endl;
    ofsTime << "Collapse time: " << dCollapseTime << "s" << endl;
    ofsTime << "Max Complex Size: " << complexSize << endl;
    ofsTime << "Max Scale: " << fMaxScale << endl;
    ofsTime.close();
    ofsTime.clear();
}
