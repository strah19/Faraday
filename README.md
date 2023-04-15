# Faraday

Faraday is a programming language written in C purposefully made for the 6502 assembly. It will emit 6502 bytecode
while also being able to run on any machine as an interpreter. 

# Utility Programs

Faraday has several utility programs for developers, such as a program lister, compressor, cross-referencer, cruncher, and uncruncher. For clarification,
the compressor outputs a source file in a state where the Lexer can still analyze it, while the cruncher outputs a source file that the Lexer cannot
understand.  However, the purpose of the cruncher is to have a file that represents the code in as little space as possible. The uncruncher then uses the cruncher
file format to output a compressed style file which the lexer can read. The file types include .com for compression, .cru for crunched files, and .unc for uncrunched
files.