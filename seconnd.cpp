/*
 * ANGEL GABRIEL CHAVEZ OTZOY
 * 24248
 * 25 / 08 / 2025
 * LABORATORIO 5: SEGUNDA PARTE
 */

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <tuple>
#include <thread>
#include <mutex>
// para medir el tiempo de ejecucion
#include <chrono>

using namespace std;

// mutex
mutex output_mutex;

// CODIGO EXTRAIDO DEL LABORATORIO 03
// https://github.com/angcoder-c/Laboratorio3-Microprocesadores/blob/main/main.cpp
// =========================================================================
int binOp2Int(string op) {
    // convertir un operador binario en entero
    int op_int = 0;
    for (int i = op.length()-1; i >=0; i--) {
        if (op[i] == '1') {
            // sumar al resultado, la potencia de dos correspondiente
            op_int += pow(2, op.length()-1-i);
        }
    }
    return op_int;
}

// READ INPUT
string getOpCode (string input) {
    // obtener el opcode en string
    return input.substr(0,3);
}

bool opInvert (string input) {
    // determinar si la operacion es invertida
    if (input[3]=='1') {
        return true;
    }
    return false;
}

int getOp1 (string input) {
    // obtener el operdor 1
    return binOp2Int(input.substr(4,2));
}

int getOp2 (string input) {
    // obtener el operador 2
    return binOp2Int(input.substr(6,2));
}

// VALIDACION DE ENTRADA

bool validarInput (string input) {
    // evaluar que el imput sea valido
    bool valid = false;
    if (input.length()!=8) {
        return false;
    }

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '1' || input[i] == '0') {
            valid = true;
        } else {
            valid = false;
        }
    }
    return valid;
}

tuple<int, string> applyOps (string opcode, int op1, int op2, bool invert) {
    /*
    000 → Suma
    001 → Resta
    010 → Multiplicación
    011 → División entera
    101 → Potencia (A^B)
    110 → A mod B
     */
    int result = 0;
    if (opcode == "000") {
        return tuple<int, string>(op1 + op2, "EXITOSO");
    } else if (opcode == "001") {
        if (invert) {
            result = op2 - op1;
        } else {
            result = op1 - op2;
        }
        return tuple<int, string>(result, "EXITOSO");;
    } else if (opcode == "010") {
        return tuple<int, string>(op1 * op2, "EXITOSO");
    } else if (opcode == "011") {
            if (invert) {
                if (op1 == 0) {
                    return tuple<int, string>(0, "ERROR DIVISION POR CERO");
                } else {
                    return tuple<int, string>(op2 / op1, "EXITOSO");
                }
            } else {
                if (op2 == 0) {
                    return tuple<int, string>(0, "ERROR DIVISION POR CERO");
                } else {
                    return tuple<int, string>(op1 / op2, "EXITOSO");
                }
            }
    } else if (opcode == "101") {
        if (invert) {
            result = pow(op2, op1);
        } else {
            result = pow(op1, op2);
        }
        return tuple<int, string>(result, "EXITOSO");
    } else if (opcode == "110") {
        if (invert) {
            if (op1 == 0) {
                return tuple<int, string>(0, "ERROR MODULO POR CERO");
            }
            result = op2 % op1;
        } else {
            if (op2 == 0) {
                return tuple<int, string>(0, "ERROR MODULO POR CERO");
            }
            result = op1 % op2;
        }
        return tuple<int, string>(result, "EXITOSO");
    }
    return tuple<int, string>(0, "ERROR OPERACION NO SOPORTADA");
}

vector<string> split(string s, char delim) {
    // separa el input por espacios en blanco
    vector<string> tokens;
    string token;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == delim) {
            tokens.push_back(token);
            token.clear();
        } else if (i+1 == s.length()) {
            token += s[i];
            tokens.push_back(token);
            token.clear();
        }
        else {
            token += s[i];
        }
    }
    return tokens;
}
// =========================================================================

// FUNCION QUE EJECUTA CADA HILO
void procesarInstruccion(string instruction, int thread_id) {
    auto start_time = chrono::high_resolution_clock::now();

    if (!validarInput(instruction)) {
        // sincronizar el output
        lock_guard<mutex> lock(output_mutex);
        cout << "===================="
             << "\nHILO ID: " << thread_id
             << "\nINSTRUCCION: " << instruction
             << "\nERROR: ENTRADA INVALIDA"
             << "\n====================" << endl;
        return;
    }

    // OBTENER PARAMETROS DE LA INSTRUCCION DE 8 BITS
    string opcode = getOpCode(instruction);
    bool invert = opInvert(instruction);
    int op1 = getOp1(instruction);
    int op2 = getOp2(instruction);
    tuple<int, string> result = applyOps(opcode, op1, op2, invert);

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    // sincronizar el output
    lock_guard<mutex> lock(output_mutex);
    cout << "===================="
         << "\nHILO ID: " << thread_id
         << "\nINSTRUCCION: " << instruction
         << "\nTIEMPO DE EJECUCION: " << duration.count() << " microsegundos"
         << "\nOPCODE: " << opcode
         << "\nINVERTIDO: " << (invert ? "SI" : "NO")
         << "\nOP1: " << op1
         << "\nOP2: " << op2
         << "\nRESULTADO: " << get<0>(result)
         << "\nMENSAJE: " << get<1>(result)
         << "\n====================" << endl;
}

int main() {
    string input_str;
    char inmenu = 'x';

    cout << "BIENVENIDO" << endl;
    cout << "Instrucciones soportadas:" << endl;
    cout << "000 -> Suma" << endl;
    cout << "001 -> Resta" << endl;
    cout << "010 -> Multiplicacion" << endl;
    cout << "011 -> Division entera" << endl;
    cout << "101 -> Potencia (A^B)" << endl;
    cout << "110 -> A mod B" << endl;
    cout << "Bit 4: 1=invertir operandos, 0=normal" << endl;
    cout << "Bits 5-6: Operando 1 (00-11)" << endl;
    cout << "Bits 7-8: Operando 2 (00-11)" << endl;

    while (inmenu != '0') {
        cout << "\nMENU\n(1) Evaluar operacion\n(0) Salir\n>>>  ";
        cin >> inmenu;
        cin.ignore();

        if (inmenu == '1') {
            cout << "Input: ";
            getline(cin, input_str);
            vector<string> instructions = split(input_str, ' ');

            if (instructions.empty()) {
                cout << "No se ingresaron instrucciones." << endl;
                continue;
            }

            // hilos
            vector<thread> threads;

            // hilo por instruccion
            for (int i = 0; i < instructions.size(); i++) {
                threads.emplace_back(procesarInstruccion, instructions[i], i + 1);
            }

            // unir los hilos con el hilo principal
            for (auto& t : threads) {
                t.join();
            }

            cout << "\nHILOS TERMINADOS." << endl;
        }

        if (inmenu == '0') {
            cout << "Bye" << endl;
        }
    }
    return 0;
}