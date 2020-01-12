...

        /**
         * Load data from a binary file into emulator memory.
         *
         * @param path The path of the file from which data should be loaded.
         * @param offset The offset in memory to which data should be loaded.
         * @return Whether the file could be opened successfully or not.
         */
        bool loadFromFile(std::string path, Address offset = 0) {
            std::ifstream file;

            file.open(path,
                      std::ios::in | // Input/read mode.
                      std::ios::binary | // Binary mode.
                      std::ios::ate); // Place cursor at end of file so that `tellg` will return file size.

            if(file.is_open() && size > offset) {
                Address fileSize = file.tellg();
                Address readSize = std::min(fileSize, size - offset); // Either the entire file will be read or a
                                                                      // number of bytes equal to the size of memory
                                                                      // available will be read (whichever is smaller).

                if(readSize > 0) {
                    file.seekg(0, std::ios::beg);
                    auto ptr = reinterpret_cast<char*>(mem.get() + offset);
                    file.read(ptr, readSize);
                }

                file.close();
                return true;
            }

            return false;
        }

        /**
         * Save all data in emulator memory to a binary file.
         *
         * @param path The path of the file to write to. Note that if a file already exists at this path, it will be
         *        overwritten.
         * @return Whether the file could be opened successfully or not.
         */
        bool saveToFile(std::string path) const {
            std::ofstream file;

            file.open(path,
                      std::ios::out | // Output/write mode.
                      std::ios::binary | // Binary mode.
                      std::ios::trunc); // Overwrite existing file contents should it already exist.

            if(file.is_open()) {
                auto ptr = reinterpret_cast<char*>(mem.get()); // Cast pointer to the raw char pointer the `write`
                                                               // method expects.
                file.write(ptr, size);

                file.close();
                return true;
            }

            return false;
        }

        const Address size;

...
