/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2019, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file RelationDataStructure.h
 *
 * Identifies the available data structures.
 ***********************************************************************/

#pragma once

namespace souffle {

/**
 * Data structures used for a relation.
 */
enum class RelationDataStructure {
    BTREE,  // btree data-structure
    BRIE,   // btree data-structure
    EQREL   // equivalence relation
};

}  // end of namespace souffle
