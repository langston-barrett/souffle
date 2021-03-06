/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file RamTranslationUnit.h
 *
 * Define a class that represents a Datalog translation unit, consisting
 * of a datalog program, error reports and cached analysis results.
 *
 ***********************************************************************/

#pragma once

#include "DebugReport.h"
#include "ErrorReport.h"
#include "RamAnalysis.h"
#include "RamProgram.h"
#include "SymbolTable.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace souffle {

/**
 * @class RamTranslationUnit
 * @brief Translating a RAM program
 *
 * Comprises the program, symbol table, error report and debug report
 */
class RamTranslationUnit {
public:
    RamTranslationUnit(std::unique_ptr<RamProgram> program, SymbolTable& sym, ErrorReport& e, DebugReport& d)
            : program(std::move(program)), symbolTable(sym), errorReport(e), debugReport(d) {}

    virtual ~RamTranslationUnit() = default;

    template <class Analysis>
    Analysis* getAnalysis() const {
        std::string name = Analysis::name;
        auto it = analyses.find(name);
        if (it == analyses.end()) {
            // analysis does not exist yet, create instance and run it.
            auto analysis = std::make_unique<Analysis>();
            analysis->run(*this);
            // Check it hasn't been created by someone else, and insert if not
            std::lock_guard<std::mutex> guard(analysisLock);
            it = analyses.find(name);
            if (it == analyses.end()) {
                analyses[name] = std::move(analysis);
            }
        }
        return dynamic_cast<Analysis*>(analyses[name].get());
    }

    const RamProgram* getProgram() const {
        return program.get();
    }

    RamProgram* getProgram() {
        return program.get();
    }

    souffle::SymbolTable& getSymbolTable() const {
        return symbolTable;
    }

    ErrorReport& getErrorReport() {
        return errorReport;
    }

    const ErrorReport& getErrorReport() const {
        return errorReport;
    }

    void invalidateAnalyses() {
        analyses.clear();
    }

    DebugReport& getDebugReport() {
        return debugReport;
    }

    const DebugReport& getDebugReport() const {
        return debugReport;
    }

protected:
    /** cached analyses */
    mutable std::map<std::string, std::unique_ptr<RamAnalysis>> analyses;

    /* Program RAM */
    std::unique_ptr<RamProgram> program;

    /* The table of symbols encountered in the input program */
    souffle::SymbolTable& symbolTable;

    ErrorReport& errorReport;

    DebugReport& debugReport;

    mutable std::mutex analysisLock;
};

}  // end of namespace souffle
