#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <clocale>
#include <sstream>
#define NOMINMAX
#include "Windows.h"
#include <limits>




class HashMap {
private:
	struct Element {
		std::string key;
		std::string value;
	};


	std::vector<Element*> map;

	static int calcIndexByHash(int hash, int mapSize) {
		return hash % mapSize;
	}

	static std::vector<Element*> rehash(std::vector<Element*>& oldMap, int newSize) {
		std::vector<Element*> newMap;
		newMap.resize(newSize);

		for (auto& el : oldMap) {
			if (el != nullptr) {
				insertInner(newMap, el);
			}
			
		}
		return newMap;
	}


	static int insertInner(std::vector<Element*>& hashMap, Element* el) {
		if (hashMap.empty()) {
			hashMap.resize(100, nullptr);
		}

		int initialIndex = calcIndexByHash(hash(el->key), hashMap.size());
		int firstEmpty = -1;

		for (int i = 0; i < hashMap.size(); i++) {
			int resIndex = (initialIndex + i * i) % hashMap.size();

			if (hashMap[resIndex] == nullptr) {
				if (firstEmpty == -1) {
					firstEmpty = resIndex;
				}
				continue;
			}

			if (hashMap[resIndex]->key == el->key) {
				if (hashMap[resIndex] != el) {
					delete hashMap[resIndex];
					hashMap[resIndex] = el;
				}
				return 1;
			}
		}

		if (firstEmpty != -1) {
			hashMap[firstEmpty] = el;
			return 1;
		}

		hashMap = rehash(hashMap, hashMap.size() + (hashMap.size()/2 + 1));
		return insertInner(hashMap, el);
	}
	
	static int removeInner(std::vector<Element*>& hashMap, std::string key, int depth) {
		if (depth >= hashMap.size()) {
			throw std::runtime_error("No key!!!");
		}
		
		int indexToDelete = ((HashMap::calcIndexByHash(hash(key), hashMap.size())) + depth * depth) % hashMap.size();
		
		if (hashMap[indexToDelete] == nullptr) {
			return removeInner(hashMap, key, depth + 1);
		}

		if (hashMap[indexToDelete]->key == key) {
			delete hashMap[indexToDelete];
			hashMap[indexToDelete] = nullptr;
			return 1;
		}

		return removeInner(hashMap, key, depth + 1);
	}

	static int hash(std::string key) {
		int result = 0;
		std::cout << "Key: " << key << std::endl;
		for (float i = 0; i < (float)key.length() / 3; i += 1) {
			std::string subKey = key.substr(i * 3, 3);
			result += std::stoi(subKey);
			std::cout << "SubKey: " << subKey << " Result: " << result << std::endl;
		}
		std::cout << "Hash result: " << result << std::endl;
		return result;
	}

public:
	~HashMap() {
		for (auto& el : map) {
			delete el;
		}
	}

	std::string get(std::string key) {
		if (map.empty()) {
			throw std::runtime_error("No key!!!");
		}
		int index = calcIndexByHash(hash(key), map.size());
		
		for (int depth = 0; depth < map.size(); depth++) {
			int resIndex = (index + depth * depth) % map.size();

			if (map[resIndex] != nullptr && map[resIndex]->key == key) {
				return map[resIndex]->value;
			}
		}

		throw std::runtime_error("No key!!!");
	}

	void set(std::string key, std::string value) {
		Element* el = new Element{ key, value };
		insertInner(map, el);
	}

	void remove(std::string key) {
		removeInner(map, key, 0);
	}

	void print() {
		for (auto& el : map) {
			if (el != nullptr) {
				std::cout << "\"" << el->key << "\": " << "\"" << el->value << "\"" << std::endl;
			}
		}
	}
};

void readHashmap(std::istream& stream, HashMap& map) {
	std::string substr;
	while (std::getline(stream, substr, ';')) {
		std::stringstream ss(substr);
		std::string key;
		std::string value;
		std::getline(ss, key, ':');
		std::getline(ss, value);

		map.set(key, value);
	}
}	

int main() {
	SetConsoleCP(CP_UTF8);       
	SetConsoleOutputCP(CP_UTF8);

	while (true) {
		try {
			std::cout << "Where do I get the key/values? \n1 - console\n2 - file\n0 - exit\n\nEnter: ";
			int switchValue;
			HashMap hashMap;
			if (!(std::cin >> switchValue)) {
				throw std::runtime_error("Invalid menu option");
			}

			switch (switchValue) {
			case 1:
			{
				std::cout << "Enter the hashmap: ";
				std::cin.ignore(
					std::numeric_limits<std::streamsize>::max(), '\n'
				);

				std::string line;
				std::getline(std::cin, line);

				std::stringstream ss(line);
				readHashmap(ss, hashMap);
				std::cout << "\nHASHMAP: \n";
				hashMap.print();

				break;
			}
			case 2: {
				std::cout << "Enter the main hashmap filename: ";
				std::string filename;
				std::cin >> filename;

				std::ifstream file(filename);

				readHashmap(file, hashMap);
				std::cout << "\nHASHMAP: \n";
				hashMap.print();
				break;
			}
			case 0: {
				return 0;
			}
			default: {
				throw std::runtime_error("Unknown menu option");
			}
			}

			std::cout << std::endl;
			std::cout << "\nPress Enter to continue...\n";
			std::cin.ignore(30, '\n');
			std::cin.get();
			system("cls");
		}
		catch (const std::exception& error) {
			std::cout << "\nError: " << error.what() << "\n\n";
			if (std::cin.eof()) {
				return 0;
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}