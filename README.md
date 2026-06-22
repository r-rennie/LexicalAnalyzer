# High-Performance Lexical Analyzer

## Project Overview
This repository contains a high-performance compiler tool engineered in C++ designed to ingest raw source code strings and systematically transform them into structured token architectures. The tool implements robust lexical scanning algorithms, precise data structures, and state-machine logic to handle language-specific keywords, syntax verification, and whitespace filtering while maintaining strict data integrity.

## Core Features & Engineering Concepts
* **Stream-Based Ingestion:** Processes raw input code sequentially, tracking character transitions to identify valid lexemes.
* **Deterministic State-Machine Logic:** Evaluates strings against language rules to isolate keywords, identifiers, literals, and operators.
* **Robust Error Handling:** Features low-level isolation algorithms to catch and report syntax anomalies and illegal characters without crashing the system execution.
* **Algorithmic Efficiency:** Optimized for fast execution speed and minimal memory footprint by utilizing lightweight, continuous data mapping.

## Why This Relates to Data Engineering
While built as a systems programming project, the core architecture mirrors a modern data ingestion and transformation pipeline (ETL):
1. **Ingest:** Reading unstructured, raw stream files into the system memory.
2. **Transform & Clean:** Parsing strings, dropping unnecessary noise (whitespace/comments), and filtering syntax anomalies.
3. **Load:** Structuring the finalized data into rigorous, typed token data structures for downstream consumption.

## Technologies Used
* **Language:** C++
* **Concepts:** Finite Automata, Relational Data Mapping, Low-Level Memory Management

## How to Run the Project
1. Clone the repository:
```bash
   git clone [https://github.com/r-rennie/LexicalAnalyzer.git](https://github.com/r-rennie/LexicalAnalyzer.git)
