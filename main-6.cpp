#include <iostream>
#include "dlList.h"
#include "css.h"
#include "node.h"
#ifdef _WIN32
#include <io.h>
#define READ(a, b, c) _read(a, b, c)
#endif
#ifdef __APPLE__
#include <unistd.h>
#define READ(a, b, c) read(a, b, c)
#endif

#define BUF_LEN (10 * 1024 * 1024)
#define RD_LEN 1024
#define CMD_START "****"
#define CSS_START "????"

template class listNode<Attribute>;
template class DoublyLinkedList<Attribute>;

template class listNode<Selector>;
template class DoublyLinkedList<Selector>;

template class listNode<Node>;
template class DoublyLinkedList<Node>;

static CSS *cssStruct = new CSS();

static char cssBuffer[BUF_LEN];
static char *cssPtr = cssBuffer;

char *StrDup(const char *str)
{
    if (str)
    {
        int len = std::strlen(str);
        char *out = (char *)malloc(len + 2);
        memset(out, 0, len + 2);
        if (len)
        {
            memcpy(out, str, len);
        }
        return out;
    }
    return NULL;
}

static char *trimback(char *str)
{
    if (str == NULL)
    {
        return str;
    }
    int len = std::strlen(str);
    if (len == 0)
    {
        return str;
    }
    char *p = str + len - 1;
    while (p >= str)
    {
        switch (*p)
        {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case '\b':
            *p = 0;
            p--;
            break;
        default:
            return str;
        }
    }
    return str;
}

static char *trimfrwd(char *str)
{
    while (str && *str)
    {
        switch (*str)
        {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case '\b':
            str++;
            break;
        default:
            return str;
        }
    }
    return str;
}

static char *strtrim(char *str)
{
    return trimfrwd(trimback(str));
}

static void parseCSSKeys(Section *sec, char *css)
{
    char *p = std::strtok(strtrim(css), ":");
    int item = 0;
    p = strtrim(p);
    while (p && *p)
    {
        char *key;
        if (item % 2 == 0)
        {
            key = p;
        }
        else
        {
            sec->addAttribute(key, p);
        }
        p = std::strtok(NULL, ":");
        p = strtrim(p);
        item++;
    }
}

static void parseCSSAtrtribute(Section *sec, char *css)
{
    char *p = std::strtok(strtrim(css), ";\n\r\t");
    p = strtrim(p);
    while (p && *p)
    {
        int len = std::strlen(p);

        parseCSSKeys(sec, p);

        p = std::strtok(p + len + 1, ";\n\r\t");
        p = strtrim(p);
    }
}

static void parseCSSSelector(Section *sec, char *css)
{
    char *p = std::strtok(strtrim(css), ",\n\r\t");
    p = strtrim(p);
    while (p && *p)
    {
        bool added = sec->addSelector(p);
        p = std::strtok(NULL, ",\n\r\t");
        p = strtrim(p);
    }
}

static void parseCSSData(char *cssData)
{
    char *cssText = StrDup(cssData);
    char *p = std::strtok(cssText, "{}");
    int item = 0;
    Section *sec = NULL;
    while (p && *p)
    {
        int len = std::strlen(p);
        char *d = StrDup(p);

        if (item % 2 == 0)
        {
            sec = cssStruct->addSection();
            parseCSSSelector(sec, strtrim(d));
        }
        else
        {
            parseCSSAtrtribute(sec, strtrim(d));
        }
        free(d);
        p = std::strtok(p + len + 1, "{}");
        item++;
    }

    free(cssText);

    if (sec && (item % 2 == 1))
    {
        int secId = cssStruct->countSections();
        if (secId)
        {
            bool del = cssStruct->deleteSection(secId);
        }
    }
}

bool parseComm(char *comm)
{
    comm = strtrim(comm);
    char *cmdCopy = StrDup(comm);
    char *p = strtok(comm, ",");
    char *first = NULL;
    char cmd;
    char *third = NULL;

    int counter = 0;
    while (p && *p)
    {
        switch (counter)
        {
        case 0:
            if (*p == '?')
            {
                std::cout << "? == " << cssStruct->countSections() << std::endl;
                return true;
            }
            first = StrDup(p);
            break;
        case 1:
            if (strlen(p) != 1)
            {
                free(first);
                free(cmdCopy);
                return false;
            }
            cmd = *p;
            break;
        case 2:
        {
            third = p;
            switch (cmd)
            {
            case 'S':
            {
                int value1 = atoi(first);
                int value3 = atoi(third);
                if (value1 && *third == '?')
                {
                    // i,S,? – wypisz liczbę selektorów dla sekcji nr i (numery zaczynają się od 1), jeśli nie ma takiego bloku pomiń;
                    Section *sec = cssStruct->findSection(value1);

                    if (sec)
                    {
                        std::cout << cmdCopy << " == " << sec->countSelectors() << std::endl;
                    }
                    else
                    {
                        free(first);
                        free(cmdCopy);
                        return false;
                    }
                }
                else if (value1 == 0 && *third == '?')
                {
                    // z,S,? – wypisz łączną (dla wszystkich bloków) liczbę wystąpień selektora z. Możliwe jest 0;
                    int count = cssStruct->countSelectors(first);
                    std::cout << cmdCopy << " == " << count << std::endl;
                }
                else if (value1 && value3)
                {
                    // i,S,j – wypisz j-ty selector dla i-tego bloku (numery sekcji oraz atrybutów zaczynają się od 1) jeśli nie ma sekcji lub selektora pomiń;
                    Section *section = cssStruct->findSection(value1);
                    if (section)
                    {
                        Selector *sel = section->findSelectorByIndex(value3);
                        if (sel)
                        {

                            std::cout << cmdCopy << " == " << sel << std::endl;
                            break;
                        }
                    }

                    free(first);
                    free(cmdCopy);
                    return false;
                }
            }
            break;
            case 'A':
            {
                int value1 = atoi(first);
                int value3 = atoi(third);
                // i,A,? - wypisz liczbę atrybutów dla sekcji nr i, jeśli nie ma takiego bloku lub sekcji pomiń;
                if (value1 && *third == '?')
                {
                    Section *sec = cssStruct->findSection(value1);
                    if (sec)
                    {
                        std::cout << cmdCopy << " == " << sec->countAttributes() << std::endl;
                    }
                    else
                    {
                        free(first);
                        free(cmdCopy);
                        return false;
                    }
                }
                else if (*third == '?')
                {
                    // n,A,? – wypisz łączną (dla wszystkich bloków) liczbę wystąpień atrybutu nazwie n. (W ramach pojedynczego bloku duplikaty powinny zostać usunięte na etapie wczytywania). Możliwe jest 0;
                    int countNum = cssStruct->countAttributes(first);
                    std::cout << cmdCopy << " == " << countNum << std::endl;
                    free(first);
                    free(cmdCopy);
                    return true;
                }
                // i,A,n – wypisz dla i-tej sekcji wartość atrybutu o nazwie n, jeśli nie ma takiego pomiń;
                else if (value1)
                {
                    Section *section = cssStruct->findSection(value1);
                    if (section && section->printAttValue(third) != NULL)
                    {
                        std::cout << cmdCopy << " == " << section->printAttValue(third) << std::endl;
                    }
                    else
                    {
                        free(first);
                        free(cmdCopy);
                        return false;
                    }
                }
            }
            break;
            case 'E': // z,E,n – wypisz wartość atrybutu o nazwie n dla selektora z, w przypadku wielu wystąpień selektora z bierzemy ostatnie. W przypadku braku pomiń;
            {
                Attribute *att = cssStruct->findAttributeInSelector(first, third);
                if (att)
                {
                    std::cout << cmdCopy << " == " << att << std::endl;
                }
                else
                {
                    free(first);
                    free(cmdCopy);
                    return false;
                }
            }
            break;
            case 'D':
            { // i,D,* - usuń całą sekcję nr i (tj. separatory+atrybuty), po poprawnym wykonaniu wypisz deleted;
                int value1 = atoi(first);
                if (first && *third == '*')
                {
                    if (cssStruct->deleteSection(value1))
                    {
                        std::cout << cmdCopy << " == "
                                  << "deleted" << std::endl;
                    }
                    else
                    {
                        free(first);
                        free(cmdCopy);
                        return false;
                    }
                }
                else
                { // i,D,n – usuń z i-tej sekcji atrybut o nazwie n, jeśli w wyniku operacji pozostaje pusta sekcja powinna zostać również usunięta (wraz z ew. selektorami), po poprawnym wykonaniu wypisz deleted.

                    if (cssStruct->deleteAttributeInSection(value1, third))
                    {
                        std::cout << cmdCopy << " == deleted" << std::endl;
                    }
                    else
                    {
                        free(first);
                        free(cmdCopy);
                        return false;
                    }
                }
            }
            break;

            default:
                break;
            }
        }
        break;
        default:
            free(first);
            free(cmdCopy);
            return false;
        }
        p = strtok(NULL, ",");
        counter++;
    }
    if (first)
    {
        free(first);
    }
    free(cmdCopy);
    return false;
}

void parseComms(char *buff)
{
    int len = 0;
    char *buffer = StrDup(buff);
    char *p = strtok(buffer, "\t\n\r");
    while (p && *p)
    {
        len = strlen(p);
        parseComm(p);
        p = strtok(p + len + 1, "\t\n\r");
    }
    free(buffer);
}

#define READ_FILE 0

int main()
{
    char *p = cssBuffer;
    int total = 0;
    int rd = 0;
#if READ_FILE
    while ((rd = fread(p + total, 1, RD_LEN, f)) > 0 && total < BUF_LEN - RD_LEN)
#else
    while ((rd = READ(0, p + total, RD_LEN)) > 0 && total < BUF_LEN - RD_LEN)
#endif
    {
        total += rd;
    }
    *(p + total) = 0;

    p = cssBuffer;
    char *cmd;
    char *css;

    do
    {
        cmd = strstr(p, CSS_START);
        css = strstr(p, CMD_START);

        if (cmd && (!css || css > cmd))
        {
            *cmd = 0;
            parseCSSData(p);
            p = cmd + 4;
        }
        else if (css && (!cmd || cmd > css))
        {
            *css = 0;
            parseComms(p);
            p = css + 4;
        }
        else if (!css && !cmd)
        {
            parseComms(p);
            break;
        }
    } while (true);

#if READ_FILE
    fclose(f);
#endif

    return 0;
}