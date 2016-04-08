/*
 * LARObj.h
 *
 *  Created on: 08 apr 2016
 *  Author: Salvati Danilo
 *  License: MIT License https://opensource.org/licenses/MIT
 *
 */

#ifndef INCLUDE_IOINTERFACES_LAROBJ_H_
#define INCLUDE_IOINTERFACES_LAROBJ_H_

#include "LARIO.h"

namespace LAR {
namespace IO {

class LARObj: public LARIO {

public:
	std::pair<std::deque<Eigen::Vector3f>,
			std::deque<Eigen::SparseMatrix<int, Eigen::RowMajor, int> > > readModel(
			std::string filePath);

	void writeModel(std::deque<Eigen::Vector3f> verticesList,
			Eigen::SparseMatrix<int, Eigen::RowMajor, int> topologicalRelationship,
			std::string outputPath);
private:
	std::vector<std::string> tokenize;

};
}
}

#endif /* INCLUDE_IOINTERFACES_LAROBJ_H_ */
