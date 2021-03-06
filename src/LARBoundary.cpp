/*
 * LARBoundary.cpp
 *
 *  Created on: 03 apr 2016
 *  Author: Salvati Danilo
 *  License: MIT License https://opensource.org/licenses/MIT
 *
 */

#include "LARBoundary.h"
#include <vector>

Eigen::SparseMatrix<int, Eigen::RowMajor, int> LAR::LARBoundary::convexUnsignedBoundary(
		Eigen::SparseMatrix<int, Eigen::RowMajor, int> CV,
		Eigen::SparseMatrix<int, Eigen::RowMajor, int> FV) {

	Eigen::SparseMatrix<int, Eigen::RowMajor, int> FC = FV * CV.transpose();

	// Return the binary sparse matrix of the boundary operator
	return boundaryFilter(FC);
}

Eigen::SparseMatrix<int, Eigen::RowMajor, int> LAR::LARBoundary::boundaryFilter(
		Eigen::SparseMatrix<int, Eigen::RowMajor, int> FC) {

	std::vector<Eigen::Triplet<int, int> > tripletList;
	int numberOfRows = FC.rows();

	Eigen::SparseMatrix<int, Eigen::RowMajor, int> result(numberOfRows,
			FC.cols());

	int FCsize = FC.outerSize();
	tripletList.reserve(numberOfRows);
	for (int k = 0; k < FCsize; ++k) {
		int maxRowValue = LAR::LARBoundary::maxRowValue(FC, k);
		for (Eigen::SparseMatrix<int, Eigen::RowMajor, int>::InnerIterator it(
				FC, k); it; ++it) {
			int value = it.value();

			if (value == maxRowValue) {
				int row = (int) it.row();
				int col = (int) it.col();
				tripletList.push_back(Eigen::Triplet<int, int>(row, col, 1));
			}
		}
	}

	result.setFromTriplets(tripletList.begin(), tripletList.end());

	return result;
}

int LAR::LARBoundary::maxRowValue(
		Eigen::SparseMatrix<int, Eigen::RowMajor, int> matrix, int rowIndex) {
	Eigen::SparseMatrix<int, Eigen::RowMajor, int>::RowXpr row = matrix.row(
			rowIndex);
	int rowSize = matrix.innerSize();
	int maxValue = 0;
	for (int i = 0; i < rowSize; i++) {
		if (row.coeff(i) > maxValue) {
			maxValue = row.coeff(i);
		}
	}

	return maxValue;
}

Eigen::SparseMatrix<int, Eigen::RowMajor, int> LAR::LARBoundary::larBoundaryChain(
		Eigen::SparseMatrix<int, Eigen::RowMajor, int> boundaryOperator,
		std::deque<int> chainList) {

	int chainSize = chainList.size();
	Eigen::SparseVector<int, Eigen::RowMajor, int> chainVector(chainSize);

	for (int i = 0; i < chainSize; i++) {
		chainVector.coeffRef(chainList[i]) = 1;
	}

	return LAR::LARBoundary::csrBinFilter(boundaryOperator * chainVector.transpose());
}

Eigen::SparseMatrix<int, Eigen::RowMajor, int> LAR::LARBoundary::csrBinFilter(
		Eigen::SparseMatrix<int, Eigen::RowMajor, int> matrix) {

	std::vector<Eigen::Triplet<int, int> > tripletList;
	int numberOfRows = matrix.rows();

	Eigen::SparseMatrix<int, Eigen::RowMajor, int> result(numberOfRows,
			matrix.cols());

	tripletList.reserve(numberOfRows);
	for (int k = 0; k < matrix.outerSize(); ++k) {
		for (Eigen::SparseMatrix<int, Eigen::RowMajor, int>::InnerIterator it(
				matrix, k); it; ++it) {
			int value = it.value();
			int row = (int) it.row();
			int col = (int) it.col();
			if (value % 2 == 1) {
				tripletList.push_back(Eigen::Triplet<int, int>(row, col, 1));
			} else if (value % 2 == -1) {
				tripletList.push_back(Eigen::Triplet<int, int>(row, col, -1));
			}
		}
	}

	result.setFromTriplets(tripletList.begin(), tripletList.end());

	return result;
}
