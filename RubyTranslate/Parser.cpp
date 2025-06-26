#include "Parser.h"
#include <iostream>

wstring Parser::openFile()
{
    errorFormat:

	wchar_t filePath[MAX_PATH] = L"";

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn)) 
	{
        wstring result = filePath;

        currentFile = result.substr(result.find_last_of(L"\\"));

        wstring currentStr = currentFile.substr(currentFile.find(L".") + 1U);

        if (currentStr == L"inc")
        {
            mode = 1U;
        }
        else if (currentStr == L"c")
        {
            mode = 2U;
        }
        else
        {
            MessageBox(NULL, L"Формат не поддерживается!", L"Внимание!", MB_OK | MB_ICONINFORMATION);

            goto errorFormat;
        }

        currentPath = result;

		return result;
	}
	else 
	{
		return L"";
	}
}

wstring Parser::saveFile()
{
    wchar_t filePath[MAX_PATH] = L"";

    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    wcscpy_s(filePath, MAX_PATH, currentFile.c_str());

    if (GetSaveFileNameW(&ofn)) 
    {
        return std::wstring(filePath);
    }
    else 
    {
        return L"";
    }
}


void Parser::parsing(const std::wstring& path)
{
    std::wifstream infile(path);
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    infile.imbue(utf8_locale);

    if (!infile.is_open())
    {
        std::wcerr << L"Не удалось открыть файл!" << std::endl;
        return;
    }

    std::wstring line;
    std::wstring currentBlock;
    std::vector<std::wstring> currentBlockPhrases;

    while (std::getline(infile, line))
    {
        size_t firstNonSpace = line.find_first_not_of(L" \t");
        std::wstring trimmed = (firstNonSpace == std::wstring::npos) ? L"" : line.substr(firstNonSpace);

        if (!trimmed.empty() && (trimmed.find(L":: @") != std::wstring::npos || trimmed.find(L": @") != std::wstring::npos))
        {
            if (!currentBlock.empty())
            {
                currentBlockPhrases.push_back(currentBlock);
                currentBlock.clear();
            }
            if (!currentBlockPhrases.empty())
            {
                phrases.push_back(currentBlockPhrases);
                currentBlockPhrases.clear();
            }
            continue;
        }

        size_t posString = trimmed.find(L".string");
        if (posString != std::wstring::npos)
        {
            size_t firstQuote = trimmed.find(L'"', posString);
            size_t lastQuote = trimmed.rfind(L'"');

            if (firstQuote != std::wstring::npos && lastQuote != std::wstring::npos && lastQuote > firstQuote)
            {
                std::wstring extracted = trimmed.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                std::wstring buffer;

                for (size_t i = 0; i < extracted.size(); ++i)
                {
                    if (extracted[i] == L'\\' && i + 1 < extracted.size())
                    {
                        if (extracted[i + 1] == L'n')
                        {
                            buffer += L'\n';
                            i++;
                        }
                        else if (extracted[i + 1] == L'p' || extracted[i + 1] == L'l')
                        {
                            buffer += L'\\';
                            buffer += extracted[i + 1];
                            i++;

                            currentBlock += buffer;
                            buffer.clear();

                            if (i + 1 < extracted.size() && extracted[i + 1] == L'$')
                            {
                                currentBlock += L'$';
                                i++;
                            }

                            if (!currentBlock.empty())
                            {
                                currentBlockPhrases.push_back(currentBlock);
                                currentBlock.clear();
                            }
                        }
                        else
                        {
                            buffer += extracted[i];
                            buffer += extracted[i + 1];
                            i++;
                        }
                    }
                    else
                    {
                        buffer += extracted[i];
                    }
                }

                currentBlock += buffer;

                if (!extracted.empty() && extracted.back() == L'$')
                {
                    if (!currentBlock.empty())
                    {
                        currentBlockPhrases.push_back(currentBlock);
                        currentBlock.clear();
                    }
                }
            }
        }
    }

    if (!currentBlock.empty())
    {
        currentBlockPhrases.push_back(currentBlock);
    }
    if (!currentBlockPhrases.empty())
    {
        phrases.push_back(currentBlockPhrases);
    }

    infile.close();

    lastBlock = phrases.size() - 1U;

    if (lastBlock == 0U)
    {
        lastBlock = 1U;
	}

    for (size_t i = 0; i < lastBlock + 1U; i++)
    {
        std::vector<std::wstring> a;
        translate.push_back(a);
    }
}

void Parser::ParsePlaceholders(const std::wstring& filePath)
{
    std::wifstream file(filePath);
    file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    if (!file.is_open())
    {
        std::wcerr << L"Не удалось открыть файл: " << filePath.c_str() << std::endl;
        return;
    }

    std::wstring line;

    while (std::getline(file, line))
    {
        auto posConst = line.find(L"const u8 ");
        if (posConst == std::wstring::npos)
            continue;

        auto posNameStart = posConst + 9;
        auto posNameEnd = line.find(L"[", posNameStart);
        if (posNameEnd == std::wstring::npos)
            continue;

        std::wstring key = line.substr(posNameStart, posNameEnd - posNameStart);

        auto posValueStart = line.find(L"_(", posNameEnd);
        if (posValueStart == std::wstring::npos)
            continue;
        posValueStart += 2;

        posValueStart = line.find(L"\"", posValueStart);
        if (posValueStart == std::wstring::npos)
            continue;
        posValueStart += 1;

        auto posValueEnd = line.find(L"\"", posValueStart);
        if (posValueEnd == std::wstring::npos)
            continue;

        std::wstring value = line.substr(posValueStart, posValueEnd - posValueStart);

        pharsersStrings.emplace_back(key, value);
    }
}


void Parser::replaceAndTransliterate(const std::wstring& inputPath, const std::wstring& outputPath, sf::RenderWindow& window)
{
    window.setTitle(L"Ruby Translator | PARSING FILE: " + inputPath);

    std::wifstream infile(inputPath);

    std::wofstream outfile(outputPath);


    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);

    infile.imbue(utf8_locale);

    outfile.imbue(utf8_locale);


    if (!infile.is_open() || !outfile.is_open()) 
    {
        std::wcerr << L"Не удалось открыть файл!" << std::endl;

        return;
    }

    std::wstring line;

    bool inBlock = false;


    size_t blockIndex = 0;

    size_t phraseIndex = 0;

    while (std::getline(infile, line)) 
    {
        size_t firstNonSpace = line.find_first_not_of(L" \t");

        std::wstring trimmed = (firstNonSpace == std::wstring::npos) ? L"" : line.substr(firstNonSpace);

        if (!trimmed.empty() && (trimmed.find(L":: @") != std::wstring::npos || trimmed.find(L": @") != std::wstring::npos)) 
        {
            inBlock = true;

            outfile << line << L"\n";

            if (blockIndex < translate.size())
            {
                phraseIndex = 0;
            }

            continue;
        }

        if (inBlock && trimmed.find(L".string") != std::wstring::npos) 
        {
            if (blockIndex < translate.size() && phraseIndex < translate[blockIndex].size())
            {
                std::wstring newText = translate[blockIndex][phraseIndex++];

                std::wstring currentLine;

                for (size_t i = 0; i < newText.size(); ++i) 
                {
                    wchar_t ch = newText[i];

                    auto it = rus_to_jpn.find(ch);

                    wchar_t outCh = (it != rus_to_jpn.end()) ? it->second : ch;

                    if (outCh == L'\n') 
                    {
                        outfile << L"  .string \"" << currentLine << L"\\n\"\n";
                        currentLine.clear();
                    }
                    else 
                    {
                        currentLine += outCh;
                    }
                }
                if (!currentLine.empty()) 
                {
                    outfile << L"  .string \"" << currentLine << L"\"\n";
                }
            }

            continue;
        }

        if (trimmed.empty()) 
        {
            if (inBlock) 
            {
                blockIndex++;
            }

            inBlock = false;
        }

        outfile << line << L"\n";
    }

    infile.close();

    outfile.close();

    window.setTitle(L"Ruby Translator");
}

void Parser::ReplaceAndCopyFile(const std::wstring& inputPath, const std::wstring& outputPath, sf::RenderWindow& window)
{
    window.setTitle(L"Ruby Translator | PARSING FILE: " + inputPath);

    std::wifstream inputFile(inputPath);
    inputFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    std::wofstream outputFile(outputPath);
    outputFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::wcerr << L"Не удалось открыть файлы." << std::endl;
        return;
    }

    std::wstring line;
    while (std::getline(inputFile, line))
    {
        auto posConst = line.find(L"const u8 ");
        if (posConst != std::wstring::npos)
        {
            auto posNameStart = posConst + 9;
            auto posNameEnd = line.find(L"[", posNameStart);
            if (posNameEnd != std::wstring::npos)
            {
                std::wstring key = line.substr(posNameStart, posNameEnd - posNameStart);

                auto it = std::find_if(
                    pharsersStrings.begin(),
                    pharsersStrings.end(),
                    [&key](const std::pair<std::wstring, std::wstring>& p) { return p.first == key; }
                );

                if (it != pharsersStrings.end())
                {
                    auto posValueStart = line.find(L"_(", posNameEnd);
                    if (posValueStart != std::wstring::npos)
                    {
                        posValueStart = line.find(L"\"", posValueStart);
                        if (posValueStart != std::wstring::npos)
                        {
                            posValueStart += 1;
                            auto posValueEnd = line.find(L"\"", posValueStart);
                            if (posValueEnd != std::wstring::npos)
                            {
                                std::wstring newValue;
                                for (wchar_t ch : it->second)
                                {
                                    auto mapIt = rus_to_jpn.find(ch);
                                    if (mapIt != rus_to_jpn.end())
                                        newValue += mapIt->second;
                                    else
                                        newValue += ch;
                                }

                                line.replace(posValueStart, posValueEnd - posValueStart, newValue);
                            }
                        }
                    }
                }
            }
        }

        outputFile << line << std::endl;
    }

    inputFile.close();
    outputFile.close();

    window.setTitle(L"Ruby Translator");
}



void Parser::start()
{
	originalTextBox.setPosition(44.5f, 474.0f);

	translateTextBox.setPosition(44.5f, 0.0f);

	writeBox.setPosition(95.0f, 237.0f);

	infoBox.setPosition(35.0f, 143.0f);
}

void Parser::update()
{
	originalTextBox.update();

	translateTextBox.update();

	writeBox.update();

    infoBox.setInfo(currentBlock + 1U, lastBlock + 1U, currentString + 1U, lastString + 1U);


	translateTextBox.setString(writeBox.getString());
}

void Parser::events(sf::Event& event, sf::RenderWindow& window)
{
	writeBox.event(event);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::F9)
		{
            phrases.clear();

            translate.clear();

            pharsersStrings.clear();


            currentBlock = 0U;

            currentString = 0U;

            wstring path = openFile();

            if (path.empty())
                return;

            if (mode == 1U)
            {
                parsing(path);

                lastString = phrases[currentBlock].size() - 1U;

                writeBox.setString(phrases[currentBlock][currentString]);
            }
            else
            {
                ParsePlaceholders(path);

                lastString = pharsersStrings.size() - 1U;

                writeBox.setString(pharsersStrings[currentString].second);
            }

            originalTextBox.setString(writeBox.getString());
		}

        if (event.key.code == sf::Keyboard::F10)
        {
            if (mode == 1U)
            {
                replaceAndTransliterate(currentPath, saveFile(), window);
            }
            else if (mode == 2U)
            {
                ReplaceAndCopyFile(currentPath, saveFile(), window);
            }
        }

        if (event.key.code == sf::Keyboard::F11)
        {
            writeBox.setString(L"");

            isSearch = true;
        }

        if (event.key.code == sf::Keyboard::Enter && isSearch)
        {
            if (mode == 1)
            {
                return;
            }
            else if (mode = 2)
            {
                size_t result = 0U;

                for (auto& str : pharsersStrings)
                {
                    if (str.second.find(writeBox.getString()) != wstring::npos)
                    {
                        break;
                    }

                    result++;
                }

                currentString = result;
            }

            writeBox.setString(pharsersStrings[currentString].second);

            originalTextBox.setString(writeBox.getString());

            isSearch = false;
        }


        if (event.key.code == sf::Keyboard::Right)
        {
            if (currentString < lastString)
            {
                currentString++;

                if (mode == 1U)
                {
                    writeBox.setString(phrases[currentBlock][currentString]);
                }
                else if (mode == 2U)
                {
                    writeBox.setString(pharsersStrings[currentString].second);
                }

                originalTextBox.setString(writeBox.getString());
            }
            else
            {
                if (currentBlock < lastBlock)
                {
                    currentBlock++;

                    currentString = 0U;

                    lastString = phrases[currentBlock].size() - 1U;

                    writeBox.setString(phrases[currentBlock][currentString]);

                    originalTextBox.setString(writeBox.getString());
                }
            }
        }

        if (event.key.code == sf::Keyboard::Left)
        {
            if (currentString > 0U)
            {
                currentString--;

                if (mode == 1U)
                {
                    writeBox.setString(phrases[currentBlock][currentString]);
                }
                else if (mode == 2U)
                {
                    writeBox.setString(pharsersStrings[currentString].second);
                }

                originalTextBox.setString(writeBox.getString());
            }
            else
            {
                if (currentBlock > 0U)
                {
                    currentBlock--;

                    lastString = phrases[currentBlock].size() - 1U;

                    currentString = lastString;

                    writeBox.setString(phrases[currentBlock][currentString]);

                    originalTextBox.setString(writeBox.getString());
                }
            }
        }


        if (event.key.code == sf::Keyboard::Up)
        {
            if (mode == 1U)
            {
                translate[currentBlock].push_back(writeBox.getString());
            }
            else if (mode == 2U)
            {
                pharsersStrings[currentString].second = writeBox.getString();
            }

            if (currentString < lastString)
            {
                currentString++;

                if (mode == 1U)
                {
                    writeBox.setString(phrases[currentBlock][currentString]);
                }
                else if (mode == 2U)
                {
                    writeBox.setString(pharsersStrings[currentString].second);
                }

                originalTextBox.setString(writeBox.getString());
            }
            else
            {
                if (currentBlock < lastBlock)
                {
                    currentBlock++;

                    currentString = 0U;

                    lastString = phrases[currentBlock].size() - 1U;

                    writeBox.setString(phrases[currentBlock][currentString]);

                    originalTextBox.setString(writeBox.getString());
                }
            }
        }
	}
}

void Parser::draw(sf::RenderWindow& window)
{
	originalTextBox.draw(window);

	translateTextBox.draw(window);

	writeBox.draw(window);

	infoBox.draw(window);
}
