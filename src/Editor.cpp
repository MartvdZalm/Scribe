#include "Editor.hpp"

#include "Highlighting.hpp"
#include "Logger.hpp"
#include "UserConfig.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>

Editor::Editor() : highlighter(UserConfig::get().getPath("syntax/knight.json").string()), currentTheme(defaultTheme)
{
    terminal.enableRawMode();

    if (config.getWindowSize() == -1)
    {
        exception.die("getWindowSize");
    }

    config.screenRows = config.screenRows - 2;
    statusMessage = "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find";
    statusMessageTime = time(nullptr);
}

void Editor::open(const std::string& filename)
{
    this->filename = filename;

    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename + " - " + std::string(std::strerror(errno)));
    }

    std::string line;
    while (std::getline(file, line))
    {
        addRow(rows.size(), line);
        // addRow(static_cast<int>(rows.size()), line);
    }

    if (file.bad())
    {
        file.close();
        throw std::runtime_error("Error reading file: " + filename + " - " + std::string(std::strerror(errno)));
    }

    file.close();

    // Ensure at least one empty row exists
    if (rows.empty())
    {
        addRow(0, "");
    }
}

void Editor::addRow(int index, std::string str)
{
    if (index < 0 || index > static_cast<int>(rows.size()))
    {
        return;
    }

    Row row;
    row.setString(str);

    if (index == static_cast<int>(rows.size()))
    {
        rows.push_back(row);
    }
    else
    {
        rows.insert(rows.begin() + index, row);
    }
}

void Editor::setPreferences(const Preferences* pref)
{
    preferences = pref;

    // Load theme if specified in preferences
    if (pref && !pref->theme.empty())
    {
        try
        {
            currentTheme = loadTheme(pref->theme);
        }
        catch (const std::exception& e)
        {
            currentTheme = defaultTheme;
            setStatusMessage("Warning: Failed to load theme, using default");
        }
    }
    else
    {
        currentTheme = defaultTheme;
    }
}

int Editor::readKey()
{
    int nread;
    char c;

    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
        {
            exception.die("read");
        }
    }

    if (c == '\x1b')
    {
        char seq[3];

        if (read(STDIN_FILENO, &seq[0], 1) != 1)
        {
            return '\x1b';
        }

        if (read(STDIN_FILENO, &seq[1], 1) != 1)
        {
            return '\x1b';
        }

        if (seq[0] == '[')
        {

            if (seq[1] >= '0' && seq[1] <= '9')
            {

                if (read(STDIN_FILENO, &seq[2], 1) != 1)
                {
                    return '\x1b';
                }

                if (seq[2] == '~')
                {
                    switch (seq[1])
                    {
                    case '1':
                        return HOME_KEY;
                    case '3':
                        return DEL_KEY;
                    case '4':
                        return END_KEY;
                    case '5':
                        return PAGE_UP;
                    case '6':
                        return PAGE_DOWN;
                    case '7':
                        return HOME_KEY;
                    case '8':
                        return END_KEY;
                    }
                }
            }
            else
            {
                switch (seq[1])
                {
                case 'A':
                    return ARROW_UP;
                case 'B':
                    return ARROW_DOWN;
                case 'C':
                    return ARROW_RIGHT;
                case 'D':
                    return ARROW_LEFT;
                case 'H':
                    return HOME_KEY;
                case 'F':
                    return END_KEY;
                }
            }
        }
        else if (seq[0] == '0')
        {
            switch (seq[1])
            {
            case 'H':
                return HOME_KEY;
            case 'F':
                return END_KEY;
            }
        }

        return '\x1b';
    }
    else
    {
        return c;
    }
}

void Editor::refreshScreen()
{
    scroll();

    std::string ab;

    ab.append("\x1b[?25l");
    ab.append("\x1b[H");

    drawRows(&ab);
    drawStatusBar(&ab);
    drawMessageBar(&ab);

    ab.append("\x1b[" + std::to_string((config.y - config.rowOff) + 1) + ";" +
              std::to_string((config.x - config.colOff) + 1) + "H");
    ab.append("\x1b[?25h");

    write(STDOUT_FILENO, ab.c_str(), ab.length());
}

void Editor::scroll()
{
    if (config.y < config.rowOff)
    {
        config.rowOff = config.y;
    }

    if (config.y >= config.rowOff + config.screenRows)
    {
        config.rowOff = config.y - config.screenRows + 1;
    }

    if (config.x < config.colOff)
    {
        config.colOff = config.x;
    }

    if (config.x >= config.colOff + config.screenCols)
    {
        config.colOff = config.x - config.screenCols + 1;
    }
}

void Editor::drawRows(std::string* ab)
{
    for (int i = 0; i < config.screenRows; i++)
    {
        int fileRow = i + config.rowOff;

        if (fileRow >= static_cast<int>(rows.size()))
        {
            if (rows.empty() && i == config.screenRows / 3)
            {
                std::string greeting = "Scribe Editor -- version " + std::string(SCRIBE_VERSION);

                int padding = (config.screenCols - greeting.length()) / 2;
                if (padding)
                {
                    ab->append("~");
                    padding--;
                }

                while (padding--)
                {
                    ab->append(" ");
                }

                ab->append(greeting);
            }
            else
            {
                ab->append("~");
            }
        }
        else
        {
            if (fileRow < 0 || fileRow >= static_cast<int>(rows.size()))
            {
                ab->append("~");
            }
            else
            {
                int len = rows[fileRow].getSize() - config.colOff;

                if (len < 0)
                {
                    len = 0;
                }

                if (len > config.screenCols)
                {
                    len = config.screenCols;
                }

                Row* row = &rows[fileRow];
                highlighter.highlight(*row);
                applyHighlighting(row, currentTheme, ab, len);
            }
        }

        ab->append("\x1b[K");
        ab->append("\r\n");
    }
}

void Editor::applyHighlighting(Row* row, HighlightTheme theme, std::string* ab, int length)
{
    if (!row || row->hl.empty())
        return;

    int rowLen = row->getSize();
    int end = std::min(config.colOff + length, rowLen);

    for (int j = config.colOff; j < end; j++)
    {
        unsigned char hl = row->hl[j];

        if (hl == HL_COMMENT)
            ab->append(theme.comment);
        else if (hl == HL_STRING)
            ab->append(theme.string);
        else if (hl == HL_NUMBER)
            ab->append(theme.number);
        else if (hl == HL_KEYWORD)
            ab->append(theme.keyword);
        else if (hl == HL_TYPE)
            ab->append(theme.type);
        else if (hl == HL_LITERAL)
            ab->append(theme.literal);
        else
            ab->append(theme.reset);

        ab->append(1, row->str[j]);
        ab->append(theme.reset);
    }
}

void Editor::drawStatusBar(std::string* ab)
{
    ab->append("\x1b[7m");

    std::string filenameDisplay = (filename.length() > 20) ? filename.substr(0, 20) : filename;
    std::string status =
        filenameDisplay + " - " + std::to_string(rows.size()) + " lines " + (dirty ? "(modified)" : "");

    ab->append(status);

    int availableSpace = config.screenCols - (status.length() + 1);

    if (availableSpace >= static_cast<int>(std::to_string(config.y + 1).length()))
    {
        while (availableSpace > static_cast<int>(std::to_string(config.y + 1).length()))
        {
            ab->append(" ");
            availableSpace--;
        }
    }

    ab->append(std::to_string(config.y + 1));
    ab->append("\x1b[m");
    ab->append("\r\n");
}

void Editor::drawMessageBar(std::string* ab)
{
    ab->append("\x1b[K");

    if (time(nullptr) - statusMessageTime < 5)
    {
        ab->append(statusMessage);
    }
}

void Editor::moveCursor(int key)
{
    Row* row = (config.y >= static_cast<int>(rows.size()) || config.y < 0) ? nullptr : &rows[config.y];

    switch (key)
    {

    case ARROW_LEFT:
    {
        if (config.x != 0)
        {
            config.x--;
        }
        else if (config.y > 0)
        {
            config.y--;
            if (config.y < static_cast<int>(rows.size()))
            {
                config.x = rows[config.y].getSize();
            }
            else
            {
                config.x = 0;
            }
        }
    }
    break;

    case ARROW_RIGHT:
    {
        if (row && config.x < row->getSize())
        {
            config.x++;
        }
        else if (row && config.x == row->getSize())
        {
            config.y++;
            config.x = 0;
        }
    }
    break;

    case ARROW_UP:
    {
        if (config.y != 0)
        {
            config.y--;
        }
    }
    break;

    case ARROW_DOWN:
    {
        if (config.y < static_cast<int>(rows.size()))
        {
            config.y++;
        }
    }
    break;
    }

    row = (config.y >= static_cast<int>(rows.size()) || config.y < 0) ? nullptr : &rows[config.y];
    int rowLen = row ? row->getSize() : 0;

    if (config.x > rowLen)
    {
        config.x = rowLen;
    }
}

void Editor::insertChar(int c)
{
    if (config.y == static_cast<int>(rows.size()))
    {
        addRow(static_cast<int>(rows.size()), "");
    }

    if (config.y < 0 || config.y >= static_cast<int>(rows.size()))
    {
        return;
    }

    Row& row = rows[config.y];
    int index = config.x;

    if (index < 0 || index > row.getSize())
    {
        index = row.getSize();
    }

    row.insertChar(index, c);
    config.x++;
    dirty++;
}

void Editor::insertNewline()
{
    if (config.y < 0 || config.y >= static_cast<int>(rows.size()))
    {
        // Add empty row at the end
        addRow(static_cast<int>(rows.size()), "");
        config.y = static_cast<int>(rows.size()) - 1;
        config.x = 0;
        return;
    }

    if (config.x == 0)
    {
        addRow(config.y, "");
    }
    else
    {
        Row& row = rows[config.y];

        std::string substring = row.getString().substr(config.x);
        row.getString().resize(config.x);
        addRow(config.y + 1, substring);
    }

    config.y++;
    config.x = 0;
    dirty++;
}

void Editor::deleteChar()
{
    if (config.y < 0 || config.y >= static_cast<int>(rows.size()))
    {
        return;
    }

    if (config.x == 0 && config.y == 0)
    {
        return;
    }

    Row& row = rows[config.y];

    if (config.x > 0)
    {
        int index = config.x - 1;

        if (index < 0 || index >= row.getSize())
        {
            return;
        }

        row.getString().erase(index, 1);
        dirty++;
        config.x--;
    }
    else
    {
        // Merge with previous row
        if (config.y > 0)
        {
            config.x = rows[config.y - 1].getSize();
            deleteRow(config.y);
            config.y--;
        }
    }
}

void Editor::deleteRow(int index)
{
    // Bounds check first before any access
    if (index < 0 || index >= static_cast<int>(rows.size()))
    {
        return;
    }

    // If not the first row, merge with previous row
    if (index > 0)
    {
        Row& prevRow = rows[index - 1];
        Row& currentRow = rows[index];

        if (currentRow.getSize() > 0)
        {
            prevRow.insertString(prevRow.getSize(), currentRow.getString());
        }
    }

    rows.erase(rows.begin() + index);
    dirty++;
}

static void findCallback(std::vector<Row>& rows, Config& config, const std::string& query, int key)
{
    static int lastMatch = -1;
    static int direction = 1;

    if (key == '\r' || key == '\x1b')
    {
        lastMatch = -1;
        direction = 1;
        return;
    }
    else if (key == ARROW_RIGHT || key == ARROW_DOWN)
    {
        direction = 1;
    }
    else if (key == ARROW_LEFT || key == ARROW_UP)
    {
        direction = -1;
    }
    else
    {
        lastMatch = -1;
        direction = 1;
    }

    if (lastMatch == -1)
    {
        direction = 1;
    }

    int current = lastMatch;

    for (int i = 0; i < static_cast<int>(rows.size()); i++)
    {
        current += direction;

        if (current == -1)
        {
            current = static_cast<int>(rows.size()) - 1;
        }
        else if (current == static_cast<int>(rows.size()))
        {
            current = 0;
        }

        if (current < 0 || current >= static_cast<int>(rows.size()))
        {
            continue;
        }

        Row row = rows[current];
        std::string rowString = row.getString();

        size_t position = rowString.find(query);

        if (position != std::string::npos)
        {
            lastMatch = current;
            config.y = current;
            config.x = static_cast<int>(position);
            // Ensure the found row is visible on screen
            if (config.y < config.rowOff)
            {
                config.rowOff = config.y;
            }
            else if (config.y >= config.rowOff + config.screenRows)
            {
                config.rowOff = config.y - config.screenRows + 1;
            }
            break;
        }
    }
}

std::string Editor::prompt(std::string promptText,
                           void (*callback)(std::vector<Row>&, Config& config, const std::string&, int))
{
    std::string input;

    while (1)
    {
        statusMessage = promptText + input;
        refreshScreen();

        int c = readKey();

        if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE)
        {
            if (!input.empty())
            {
                input.pop_back();
            }
        }
        else if (c == '\x1b')
        {
            statusMessage = "";

            if (callback)
            {
                callback(rows, config, input, c);
            }
            return "";
        }
        else if (c == '\r')
        {
            statusMessage = "";

            if (callback)
            {
                callback(rows, config, input, c);
            }
            return input;
        }
        else if (!iscntrl(c) && c < 128)
        {
            input.push_back(static_cast<char>(c));
        }

        if (callback)
        {
            callback(rows, config, input, c);
        }
    }
}

void Editor::find()
{
    int savedCx = config.x;
    int savedCy = config.y;
    int savedRowOff = config.rowOff;
    int savedColOff = config.colOff;

    std::string query = prompt("Search: ", &findCallback);

    if (query.empty())
    {
        config.x = savedCx;
        config.y = savedCy;
        config.rowOff = savedRowOff;
        config.colOff = savedColOff;
    }
}

void Editor::processKeypress()
{
    static int quitTimes = SCRIBE_QUIT_TIMES;
    int c = readKey();

    switch (c)
    {
    case '\r':
    {
        insertNewline();
    }
    break;

    case CTRL_KEY('q'):
    {
        if (dirty != 0 && quitTimes > 0)
        {
            setStatusMessage("WARNING!!! File has unsaved changes. Press Ctrl-Q " + std::to_string(quitTimes) +
                             " more times to quit.");
            quitTimes--;
            return;
        }

        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        terminal.disableRawMode();
        exit(0);
    }
    break;

    case CTRL_KEY('s'):
    {
        if (filename == "[No Name]")
        {
            std::string name = prompt("Save as: ", NULL);

            if (!name.empty())
            {
                filename = name;
            }
            else
            {
                statusMessage = "Save aborted";
                return;
            }
        }

        save();
    }
    break;

    case HOME_KEY:
    {
        config.x = 0;
    }
    break;

    case END_KEY:
    {
        if (config.y >= 0 && config.y < static_cast<int>(rows.size()))
        {
            config.x = rows[config.y].getSize();
        }
        else
        {
            config.x = 0;
        }
    }
    break;

    case CTRL_KEY('f'):
    {
        find();
    }
    break;

    case BACKSPACE:
    case CTRL_KEY('h'):
    case DEL_KEY:
    {
        if (c == DEL_KEY)
        {
            moveCursor(ARROW_RIGHT);
        }
        deleteChar();
    }
    break;

    case PAGE_UP:
    case PAGE_DOWN:
    {
        int times = config.screenRows;
        while (times--)
        {
            moveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
        }
    }
    break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
    {
        moveCursor(c);
    }
    break;

    case CTRL_KEY('l'):
    case '\x1b':
        break;

    default:
    {
        insertChar(c);
    }
    break;
    }

    quitTimes = SCRIBE_QUIT_TIMES;
}

void Editor::save()
{
    if (filename == "[No Name]")
    {
        setStatusMessage("No filename specified. Use Ctrl-S and enter a filename first.");
        return;
    }

    std::filesystem::path filePath;

    // Handle absolute paths
    if (filename[0] == '/' || (filename.length() > 1 && filename[1] == ':'))
    {
        filePath = filename;
    }
    else
    {
        filePath = std::filesystem::current_path() / filename;
    }

    std::ofstream outputFile(filePath.string(), std::ios::binary);

    if (!outputFile.is_open())
    {
        setStatusMessage("Can't save! I/O error: " + std::string(std::strerror(errno)));
        return;
    }

    for (Row& row : this->rows)
    {
        outputFile << row.getString() << '\n';

        if (outputFile.fail())
        {
            outputFile.close();
            setStatusMessage("Error writing to file: " + std::string(std::strerror(errno)));
            return;
        }
    }

    outputFile.close();

    if (outputFile.fail())
    {
        setStatusMessage("Error closing file: " + std::string(std::strerror(errno)));
        return;
    }

    // Get file size safely
    try
    {
        if (std::filesystem::exists(filePath))
        {
            std::uintmax_t fileSize = std::filesystem::file_size(filePath);
            setStatusMessage(std::to_string(fileSize) + " bytes written to disk");
        }
        else
        {
            setStatusMessage("File saved (size unknown)");
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        setStatusMessage("File saved (could not determine size)");
    }

    dirty = 0;
}

void Editor::setStatusMessage(std::string message)
{
    statusMessage = message;
    statusMessageTime = time(nullptr);
}
