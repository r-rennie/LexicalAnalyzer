# High-Performance Lexical Analyzer

## Project Overview
This repository contains a lexical analyzer engineered in C++ designed to ingest raw source code streams and systematically transform them into structured token architectures. The tool implements deterministic state-machine logic, precise collection data structures, and robust regular expression pattern matching to isolate language-specific keywords, process string literals, and filter out syntactical noise.

## System Architecture & Processing Stages
The core engine utilizes a sequential stream-processing architecture divided into three clear logical phases:

1. Ingestion & Lookahead Validation: The system reads the raw text input sequentially, utilizing lookahead logic to intercept and evaluate potential formatting anomalies (such as invalid numerical strings containing multiple decimal points) before standard pattern matching occurs.
2. Tokenization & Normalization: Utilizing an ordered chain of regular expressions, the engine matches valid text sequences against strict language syntax definitions. Input character data is normalized to a uniform case to eliminate case-sensitivity runtime mismatches against the internal keyword dictionary.
3. Categorization & Aggregation: Discovered lexemes are instantiated as individual typed objects, and structural metrics are dynamically aggregated to generate processing summaries and isolated error records upon execution completion.

## Key Technical Features
* Deterministic Matching: Features structured regular expressions prioritized to accurately differentiate complex string structures, such as multi-character operators (==, !=) versus basic assignment and relational symbols.
* Graceful Exception Isolation: Implements custom error-handling routines that capture invalid character inputs and record illegal formatting variants without disrupting the continuity of the main parsing loop.
* Memory Efficiency: Built using native C++ standard library structures (std::vector, std::unordered_map) to keep memory allocation optimized during high-frequency data evaluations.

## How to Compile and Run (Command Line)

To run this project manually from your terminal, follow these steps:

1. Clone the repository:
```bash
git clone https://github.com/r-rennie/LexicalAnalyzer.git
```

2. Navigate into the project directory:
```bash
cd LexicalAnalyzer
   ```

3. Compile the source code using g++:
```bash
g++ -o lexer main.cpp
```   

4. Execute the binary:
```bash
./lexer
```

## Environment & Run Configuration Notes

Because this program uses relative file paths to locate input source streams (e.g., text4.txt), execution is highly dependent on your environment's active Working Directory:

* If running via Terminal: Ensure your target text files are located in the root LexicalAnalyzer directory where you execute the ./lexer command.
* If running via an IDE (like CLion/CMake): The IDE builds and executes the program inside the cmake-build-debug directory by default. Ensure your sample data input files (text1.txt through text5.txt) are copied into that directory, or adjust your IDE's Run Configurations -> Working Directory to point directly to the project root folder to avoid file stream read errors.