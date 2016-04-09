/*
 * LARObj.cpp
 *
 *  Created on: 08 apr 2016
 *  Author: Salvati Danilo
 *  License: MIT License https://opensource.org/licenses/MIT
 *
 */
#include "LARObj.h"
#include "LARcpp.h"
#include <iostream>
#include <string>
#include <fstream>

std::pair<std::deque<Eigen::Vector3f>,
		std::deque<Eigen::SparseMatrix<int, Eigen::RowMajor, int> > > LAR::IO::LARObj::readModel(
		const std::string filePath) {

	std::string line;
	std::ifstream myfile;
        myfile.open(filePath, std::ifstream::in);

	std::deque<Eigen::Vector3f> vectorList;
	std::deque<Eigen::SparseMatrix<int, Eigen::RowMajor, int> > relationships;
	std::deque<std::vector<int> > facesList;
	if (myfile.is_open()) {
		std::vector<std::string> tokens;
		while (std::getline(myfile, line)) {
			tokens = tokenize(line, " ");
			if (tokens[0] == "v") {
				// I am reading a vertex line
                                float coordinatesArray[] = {atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str())};
                                std::vector<float> coordinates(coordinatesArray, coordinatesArray + sizeof(coordinatesArray) / sizeof(float));
				vectorList.push_back(
						Eigen::Vector3f(coordinates.data()));
			} else {
				// I am reading a face line
                                int faceArray[] = {atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), atoi(tokens[3].c_str())};
                                std::vector<int> face(faceArray, faceArray + sizeof(faceArray) / sizeof(int));
				facesList.push_back(face);
			}
			myfile.close();
		}
	} else {
		throw "Unable to open file " + filePath;
	}

	LAR::LARcpp larcpp;
	relationships.push_back(larcpp.brcToMatrix(facesList));
	return std::pair<std::deque<Eigen::Vector3f>,
		std::deque<Eigen::SparseMatrix<int, Eigen::RowMajor, int> > > (vectorList, relationships);
}

void LAR::IO::LARObj::writeModel(std::deque<Eigen::Vector3f> verticesList,
		Eigen::SparseMatrix<int, Eigen::RowMajor, int> topologicalRelationship,
		std::string outputPath) {

}

/** tokenizer function for strings **/
std::vector<std::string> LAR::IO::LARObj::tokenize(const std::string& str,
		const std::string& delimiters = " ") {

	std::vector<std::string> tokens;

	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// Find first "non-delimiter".
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}