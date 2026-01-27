#include <iostream>
#include <utility>
#include <stdexcept>
#include <map>
#include <string>
#include <fstream>
#include <cctype>
#include <bitset>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char ch, int freq)
        : ch(ch), freq(freq), left(nullptr), right(nullptr) {}

    Node(char ch, int freq, Node* left, Node* right)
        : ch(ch), freq(freq), left(left), right(right) {}
};

class PriorityQueue {
private:
    Node** arr;
    int capacity;
    int count;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i > 0 && arr[parent(i)]->freq > arr[i]->freq) {
            swap(arr[i], arr[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int smallest = i;
        int l = left(i);
        int r = right(i);

        if (l < count && arr[l]->freq < arr[smallest]->freq)
            smallest = l;
        if (r < count && arr[r]->freq < arr[smallest]->freq)
            smallest = r;

        if (smallest != i) {
            swap(arr[i], arr[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    PriorityQueue(int cap = 256) : capacity(cap), count(0) {
        arr = new Node*[capacity];
    }

    ~PriorityQueue() {
        delete[] arr;
    }

    bool isEmpty() { return count == 0; }
    int size() { return count; }

    void push(Node* node) {
        if (count == capacity)
            throw runtime_error("Osiągnięto limit pojemności kolejki");

        arr[count] = node;
        heapifyUp(count);
        count++;
    }

    Node* top() {
        if (isEmpty()) throw out_of_range("Kolejka pusta");
        return arr[0];
    }

    Node* pop() {
        if (isEmpty()) throw out_of_range("Kolejka pusta");

        Node* root = arr[0];
        arr[0] = arr[count - 1];
        count--;

        if (!isEmpty())
            heapifyDown(0);

        return root;
    }

    void build(Node** nodes, int n) {
        if (n > capacity) throw runtime_error("Zbyt wiele elementów");

        for (int i = 0; i < n; i++)
            arr[i] = nodes[i];

        count = n;

        for (int i = parent(count - 1); i >= 0; i--)
            heapifyDown(i);
    }

    void printHeap() {
        for (int i = 0; i < count; i++) {
            cout << "(" << arr[i]->ch << ", " << arr[i]->freq << ") ";
        }
        cout << endl;
    }
};

void freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

class HuffmanCode {
private:
    Node* root = nullptr;

public:
    ~HuffmanCode() {
        freeTree(root);
    }

    int verifyFile(ifstream &file) {
        if (!file.is_open()) {
            cerr << "Nie udało się otworzyć pliku\n";
            return 1;
        }
        return 0;
    }

    string readInput(const string& filename) {
        ifstream file(filename);

        if (verifyFile(file)) {
            return "";
        }

        string line, text;
        while (getline(file, line)) {
            text += line;
            text += '\n';
        }

        if (!text.empty() && text.back() == '\n') {
            text.pop_back();
        }

        file.close();
        return text;
    }

    void printCodes(Node* node, string code, map<char, string>& huffmanCode) {
        if (node == nullptr)
            return;

        if (!node->left && !node->right) {
            huffmanCode[node->ch] = code;
        }

        printCodes(node->left,  code + "0", huffmanCode);
        printCodes(node->right, code + "1", huffmanCode);
    }

    map<char, int> createFrequencyMap(const string& text) {
        map<char, int> freq_map;
        for (char letter : text) {
            if (isalpha(static_cast<unsigned char>(letter))) {
                letter = static_cast<char>(toupper(static_cast<unsigned char>(letter)));
            }
            freq_map[letter]++;
        }
        return freq_map;
    }

    map<char, string> buildHuffmanTree(const map<char, int>& freq_map) {
    PriorityQueue pq;

    int n = freq_map.size();
    if (n == 0) {
        root = nullptr;
        return {};
    }
        
    Node** nodes = new Node*[n];
    int i = 0;
    for (auto& pair : freq_map) {
        nodes[i++] = new Node(pair.first, pair.second);
    }

    pq.build(nodes, n);

    while (pq.size() > 1) {
        Node* left = pq.pop();
        Node* right = pq.pop();

        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }

    root = pq.top();

    map<char, string> huffmanCode;
    printCodes(root, "", huffmanCode);

    delete[] nodes; // sprzątanie

    return huffmanCode;
}

    string binaryStringToASCII(const string& binaryString) {
      string text;

      int paddingBits = (8 - binaryString.size() % 8) % 8;

      // zapisz padding jako pierwszy bajt
      text += static_cast<char>(paddingBits);

      string padded = binaryString;
      for (int i = 0; i < paddingBits; ++i) {
        padded += '0';
      }

      for (size_t i = 0; i < padded.size(); i += 8) {
        bitset<8> bits(padded.substr(i, 8));
        text += static_cast<char>(bits.to_ulong());
      }

      return text;
    }



    string encode(const string& text, const map<char, string>& huffmanCode) {
        cout << "\nOryginalny tekst:\n" << text << "\n";

        string encoded;
        for (char ch : text) {
            char c = ch;
            if (isalpha(static_cast<unsigned char>(c))) {
                c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
            }
            if (huffmanCode.count(c)) {
                encoded += huffmanCode.at(c);
            }
        }
        cout << "\nTekst zakodowany binarnie:\n" << encoded << "\n";
        return binaryStringToASCII(encoded);
    }

    string ASCIITobinaryString(const string& ascii) {
      if (ascii.empty()) return "";

      string binary;

      // pierwszy bajt = liczba bitów paddingu
      int paddingBits = static_cast<unsigned char>(ascii[0]);

      for (size_t i = 1; i < ascii.size(); ++i) {
        bitset<8> bits(static_cast<unsigned char>(ascii[i]));
        binary += bits.to_string();
      }

      if (paddingBits > 0 && paddingBits <= 7) {
        binary.resize(binary.size() - paddingBits);
      }

      return binary;
    }



    string decodeWithRoot(const string& coded_text, Node* decodeRoot) {
        if (!decodeRoot) return "";

        string coded_binary_text = ASCIITobinaryString(coded_text);

        string result;
        Node* curr = decodeRoot;

        for (char bit : coded_binary_text) {
            if (bit == '0') curr = curr->left;
            else if (bit == '1') curr = curr->right;
            else continue;

            if (!curr->left && !curr->right) {
                result += curr->ch;
                curr = decodeRoot;
            }
        }
        return result;
    }

    void saveDictionary(const map<char, string>& huffmanCode,
                        const string& filename) {
        ofstream out(filename);
        if (!out.is_open()) {
            cerr << "Nie udało się otworzyć pliku "<< filename << " do zapisu\n";
            return;
        }

        for (const auto& p : huffmanCode) {
            char ch = p.first;
            string code = p.second;

            if (ch == '\n') {
                out << "\\n:" << code << ";";
            } else if (ch == ' ') {
                out << "\\s:" << code << ";";
            } else if (ch == '\t') {
                out << "\\t:" << code << ";";
            } else {
                out << ch << ":" << code << ";";
            }
        }
        out << "\n";

        out.close();
    }
    pair<string, string> readDictionaryAndBinary(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file.is_open()) {
            cerr << "Nie udało się otworzyć pliku " << filename << "\n";
            return {};
        }

        string dictionary;
        char c;

        // czytaj do znaku nowej linii — słownik
        while (file.get(c)) {
            if (c == '\n') break;
            dictionary += c;
        }

        // reszta pliku = dane binarne
        string binaryData(
            (istreambuf_iterator<char>(file)),
            istreambuf_iterator<char>()
        );

        file.close();
        return {dictionary, binaryData};
    }

    map<char,string> loadDictionaryFromString(const string& line) {
        map<char,string> dict;

        size_t pos = 0;
        string tmp = line;

        while ((pos = tmp.find(';')) != string::npos) {
            string token = tmp.substr(0, pos);
            tmp.erase(0, pos + 1);

            auto sep = token.find(':');
            if (sep == string::npos) continue;

            char ch;

            if (token[0] == '\\') {
                if (token[1] == 'n') ch = '\n';
                else if (token[1] == 's') ch = ' ';
                else if (token[1] == 't') ch = '\t';
                else continue;
            } else {
                ch = token[0];
            }

        string code = token.substr(sep + 1);
        dict[ch] = code;
    }

    return dict;
    }


    string extractDictionaryFromCodedFile(string filename) {
        ifstream file(filename);

        if (verifyFile(file)) {
            return "";
        }

        string dict;
        getline(file, dict); //wyciągam pierwszy wiersz pliku - słownik
        return dict;
    }

    Node* buildTreeFromDictionary(const map<char, string>& dict) {
        Node* newRoot = new Node('\0', 0);
        for (const auto& p : dict) {
            char ch = p.first;
            const string& code = p.second;

            Node* curr = newRoot;
            for (char bit : code) {
                if (bit == '0') {
                    if (!curr->left) curr->left = new Node('\0', 0);
                    curr = curr->left;
                } else if (bit == '1') {
                    if (!curr->right) curr->right = new Node('\0', 0);
                    curr = curr->right;
                }
            }
            curr->ch = ch;
        }
        return newRoot;
    }

    void freeDecodedTree(Node* decodeRoot) {
        freeTree(decodeRoot);
    }
};


class UI {
public:
    void displayMenu() {
        cout << "\nKodowanie Huffmana:\n";
        cout << "1. Zakoduj tekst\n";
        cout << "2. Dekoduj tekst\n";
        cout << "3. Wyjście\n";
    }

    int chooseOption() {
        cout << "\nWybierz opcję (1-3): ";
        int choice;
        cin >> choice;
        return choice;
    }

    string getFilename() {
        cout << "\nPodaj nazwę pliku: ";
        string filename;
        cin >> filename;
        return filename;
    }
};

int main() {
    HuffmanCode hc;
    UI ui;

    while (true) {
        ui.displayMenu();
        int option = ui.chooseOption();

        if (option == 3) {
            break;
        }
        else if (option == 1) {

            string key_text = hc.readInput("key.txt");
            if (key_text.empty()) {
                cerr << "key.txt jest pusty lub nie udało się go odczytać\n";
                continue;
            }

            auto freqMap = hc.createFrequencyMap(key_text);
            cout << "\n\nLiczba wystąpień:\n";
            for (const auto& pair : freqMap) {
                cout << "'" << (pair.first == '\n' ? '\\' : pair.first) << "': " << pair.second << '\n';
            }

            auto codes = hc.buildHuffmanTree(freqMap);
            cout << "\nKody Huffmana:\n";
            for (const auto& p : codes) {
                cout << "'" << (p.first == '\n' ? '\\' : p.first) << "' -> " << p.second << '\n';
            }

            string filename = ui.getFilename();

            string text_to_encode = hc.readInput(filename);
            if (text_to_encode.empty()) {
                cerr << "Plik " << filename << " jest pusty lub nie udało się go odczytać\n";
                continue;
            }

            string encoded = hc.encode(text_to_encode, codes);
            cout << "\nZakodowany tekst:\n" << encoded << "\n";


            ofstream out("encoded.txt", ios::binary);
            if (!out.is_open()) {
                cerr << "Nie udało się otworzyć encoded.txt do zapisu\n";
            } else {

            for (const auto& p : codes) {
                char ch = p.first;
                const string& code = p.second;

                if (ch == '\n') out << "\\n:" << code << ";";
                else if (ch == ' ') out << "\\s:" << code << ";";
                else if (ch == '\t') out << "\\t:" << code << ";";
                else out << ch << ":" << code << ";";
            }
            out << '\n';

    
            out.write(encoded.data(), encoded.size());
            out.close();
        }

        cout << "\nZapisano słownik + dane binarne do encoded.txt\n";

        }
        else if (option == 2) {

            string filename = ui.getFilename();

            auto [dictLine, binaryData] = hc.readDictionaryAndBinary(filename);

            auto dict = hc.loadDictionaryFromString(dictLine);

            Node* decodeRoot = hc.buildTreeFromDictionary(dict);

            string decoded = hc.decodeWithRoot(binaryData, decodeRoot);

            hc.freeDecodedTree(decodeRoot);

            ofstream out("decoded.txt");
            if (out.is_open()) { out << decoded; out.close(); }
            cout << "Dekodowany tekst: " << decoded << "\n";

            cout << "\nZapisano dekodowany tekst do decoded.txt\n";
        }

    }

    return 0;
}
