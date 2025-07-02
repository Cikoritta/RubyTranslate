#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "TextBox.h"
#include "WritreBox.h"
#include "InfoBox.h"

#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <locale>
#include <codecvt>
#include <unordered_map>

class Parser
{
	TextBox		originalTextBox;

	TextBox		translateTextBox;


	WritreBox	writeBox;

	InfoBox		infoBox;


	wstring openFile();

	wstring saveFile();

    wstring currentFile;

    wstring currentPath;


    std::unordered_map<wchar_t, wchar_t> rus_to_jpn = {
    {L'Я', L'ッ'},
    {L'Ю', L'ポ'},
    {L'Э', L'ペ'},
    {L'Ь', L'プ'},
    {L'Ы', L'ピ'},
    {L'Ъ', L'パ'},
    {L'Щ', L'ボ'},
    {L'Ш', L'ベ'},
    {L'Ч', L'ブ'},
    {L'Ц', L'ビ'},
    {L'Х', L'バ'},
    {L'Ф', L'ド'},
    {L'У', L'デ'},
    {L'Т', L'ヅ'},
    {L'С', L'ヂ'},
    {L'Р', L'ダ'},
    {L'П', L'ゾ'},
    {L'О', L'ゼ'},
    {L'Н', L'ズ'},
    {L'М', L'ジ'},
    {L'Л', L'ザ'},
    {L'К', L'ゴ'},
    {L'Й', L'ゲ'},
    {L'И', L'グ'},
    {L'З', L'ギ'},
    {L'Ж', L'ガ'},
    {L'Ё', L'ョ'},
    {L'Е', L'ュ'},
    {L'Д', L'ャ'},
    {L'Г', L'ォ'},
    {L'В', L'ェ'},
    {L'Б', L'ゥ'},
    {L'А', L'ィ'},
    {L'я', L'ァ'},
    {L'ю', L'ン'},
    {L'э', L'ヲ'},
    {L'ь', L'リ'},
    {L'ы', L'ラ'},
    {L'ъ', L'ヨ'},
    {L'щ', L'ユ'},
    {L'ш', L'ヤ'},
    {L'ч', L'モ'},
    {L'ц', L'メ'},
    {L'х', L'ム'},
    {L'ф', L'ミ'},
    {L'у', L'マ'},
    {L'т', L'ホ'},
    {L'с', L'ヘ'},
    {L'р', L'フ'},
    {L'п', L'ヒ'},
    {L'о', L'ハ'},
    {L'н', L'ノ'},
    {L'м', L'ヌ'},
    {L'л', L'ニ'},
    {L'к', L'ナ'},
    {L'й', L'ト'},
    {L'и', L'テ'},
    {L'з', L'ツ'},
    {L'ж', L'チ'},
    {L'ё', L'タ'},
    {L'е', L'ソ'},
    {L'д', L'セ'},
    {L'г', L'ぽ'},
    {L'в', L'ぺ'},
    {L'б', L'ぷ'},
    {L'а', L'ぴ'}
    };

    std::vector<std::pair<std::wstring, std::wstring>> pharsersStrings;


    std::vector<std::vector<std::wstring>> phrases;

    std::vector<std::vector<std::wstring>> translate;


    size_t lastBlock = 0U;

    size_t currentBlock = 0U;


    size_t lastString = 0U;

    size_t currentString = 0U;


	unsigned int mode = 0U;


    bool isSearch = false;

public:


	void parsing(const wstring& path);

    void ParsePlaceholders(const std::wstring& filePath);

    void replaceAndTransliterate(const std::wstring& inputPath, const std::wstring& outputPath, sf::RenderWindow& window);

    void ReplaceAndCopyFile(const std::wstring& inputPath, const std::wstring& outputPath, sf::RenderWindow& window);


    void preTranslate(const wstring& string);


	void start();

	void update();

	void events(sf::Event& event, sf::RenderWindow& window);

	void draw(sf::RenderWindow& window);
};
