#include "../include/syntax_highlighting.h"
#include "../include/config.h"
#include "../include/terminal.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_hexa_digit(char chr) { return ('a' <= chr && chr <= 'f') || ('A' <= chr && chr <= 'F'); }

int is_separator(int chr)
{
    return isspace(chr) || chr == '\0' || strchr(",.()+-/*=~%<>[];:{}", chr) != NULL;
}

void editorUpdateSyntax(struct editorConfig* editor, erow* row)
{
    unsigned char *new_hl = realloc(row->hl, row->rsize);
    if (new_hl == NULL) {
        die("realloc failed in editorUpdateSyntax");
        return;
    }
    row->hl = new_hl;
    memset(row->hl, HL_NORMAL, row->rsize);

    if (editor->syntax == NULL){
        return;
    }

    const char** keywords = editor->syntax->keywords;

    const char* scs = editor->syntax->singleline_comment_start;
    const char* mcs = editor->syntax->multiline_comment_start;
    const char* mce = editor->syntax->multiline_comment_end;

    int scs_len = scs ? (int)strlen(scs) : 0;
    int mcs_len = mcs ? (int)strlen(mcs) : 0;
    int mce_len = mce ? (int)strlen(mce) : 0;

    int prev_sep = 1;
    char in_string = 0;
    int in_comment = (row->idx > 0 && editor->row[row->idx - 1].hl_open_comment);

    int row_index = 0;
    while (row_index < row->rsize)
    {
        char current_char = row->render[row_index];
        unsigned char prev_hl = (row_index > 0) ? row->hl[row_index - 1] : HL_NORMAL;
        char prev_c = (row_index > 0) ? row->render[row_index - 1] : '\0';

        if (scs_len && !in_string && !in_comment)
        {
            if (!strncmp(&row->render[row_index], scs, scs_len))
            {
                memset(&row->hl[row_index], HL_COMMENT, row->rsize - row_index);
                break;
            }
        }

        if (mcs_len && mce_len && !in_string)
        {
            if (in_comment)
            {
                row->hl[row_index] = HL_MLCOMMENT;
                if (!strncmp(&row->render[row_index], mce, mce_len))
                {
                    memset(&row->hl[row_index], HL_MLCOMMENT, mce_len);
                    row_index += mce_len;
                    in_comment = 0;
                    prev_sep = 1;
                }
                else
                {
                    row_index++;
                    continue;
                }
            }
            else if (!strncmp(&row->render[row_index], mcs, mcs_len))
            {
                memset(&row->hl[row_index], HL_MLCOMMENT, mcs_len);
                row_index += mcs_len;
                in_comment = 1;
                continue;
            }
        }

        if (editor->syntax->flags & HL_HIGHLIGHT_STRINGS)
        {
            if (in_string)
            {
                row->hl[row_index] = HL_STRING;
                if (current_char == '\\' && row_index + 1 < row->rsize)
                {
                    row->hl[row_index + 1] = HL_STRING;
                    row_index += 2;
                    continue;
                }
                if (current_char == in_string) {
                    in_string = 0;
                }
                row_index++;
                prev_sep = 1;
                continue;
            }
            
            if (current_char == '"' || current_char == '\'')
            {
                in_string = current_char;
                row->hl[row_index] = HL_STRING;
                row_index++;
                continue;
            }
        }

        if (editor->syntax->flags & HL_HIGHLIGHT_NUMBERS)
        {
            if ((isdigit(current_char) && (prev_sep || prev_hl == HL_NUMBER)) ||
                (current_char == '.' && prev_hl == HL_NUMBER) ||
                (current_char == 'x' && prev_hl == HL_NUMBER && prev_c == '0') ||
                (current_char == 'b' && prev_hl == HL_NUMBER && prev_c == '0') ||
                (is_hexa_digit(current_char) && prev_hl == HL_NUMBER))
            {
                row->hl[row_index] = HL_NUMBER;
                row_index++;
                prev_sep = 0;
                continue;
            }
            if ((isdigit(current_char) && prev_c == 'o'))
            {
                row->hl[row_index - 1] = HL_NUMBER;
                row->hl[row_index] = HL_NUMBER;
                row_index++;
                prev_sep = 0;
                continue;
            }
        }

        if (prev_sep)
        {
            int keywords_index = 0;
            for (keywords_index = 0; keywords[keywords_index]; keywords_index++)
            {
                size_t klen = strlen(keywords[keywords_index]);
                int kw2 = keywords[keywords_index][klen - 1] == '|';
                if (kw2){
                    klen--;
                }

                if (!strncmp(&row->render[row_index], keywords[keywords_index], klen) &&
                    is_separator(row->render[row_index + klen]))
                {
                    memset(&row->hl[row_index], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
                    row_index += (int)klen;
                    break;
                }
            }
            if (keywords[keywords_index] != NULL)
            {
                prev_sep = 0;
                continue;
            }
        }

        prev_sep = is_separator(current_char);
        row_index++;
    }

    int changed = (row->hl_open_comment != in_comment);
    row->hl_open_comment = in_comment;
    if (changed && row->idx + 1 < editor->numrows){
        editorUpdateSyntax(editor, &editor->row[row->idx + 1]);
    }
}

void editorSelectSyntaxHighlight(struct editorConfig* editor)
{
    editor->syntax = NULL;
    if (editor->filename == NULL) {
        return;
    }

    char* ext = strrchr(editor->filename, '.');

    for (unsigned int syntax_index = 0; syntax_index < hldb_entries; syntax_index++)
    {
        struct editorSyntax* syntax_p = &hldb[syntax_index];
        unsigned int language_type_index = 0;
        while (syntax_p->filematch[language_type_index])
        {
            int is_ext = (syntax_p->filematch[language_type_index][0] == '.');
            if ((is_ext && ext && !strcmp(ext, syntax_p->filematch[language_type_index])) ||
                (!is_ext && strstr(editor->filename, syntax_p->filematch[language_type_index])))
            {
                editor->syntax = syntax_p;
                return;

                int filerow = 0;
                for (filerow = 0; filerow < editor->numrows; filerow++)
                {
                    editorUpdateSyntax(editor, &editor->row[filerow]);
                }
            }
            language_type_index++;
        }
    }
}