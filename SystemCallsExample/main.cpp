/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CreateDirectoryA():
// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya
//
// GetLastError():
// https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
//
// CreateFileA():
// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
//
// WriteFile():
// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
//
// CloseHandle():
// https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <locale.h>     // necessário para setlocale()
#include <windows.h>    // necessário para funções referentes a serviços

#define     FIAPDEV_OK                  0
#define     FIAPDEV_DIR_ERROR           -1
#define     FIAPDEV_BIN_DIR_ERROR       -2
#define     FIAPDEV_LIB_DIR_ERROR       -3
#define     FIAPDEV_INI_ERROR           -4
#define     FIAPDEV_INI_CREATE_ERROR    -5
#define     FIAPDEV_INI_WRITE_ERROR     -6

int main(int argc, char * argv[])
{
    BOOL bRetorno;
    DWORD dwErro;
    char FIAPDEV_DIRECTORY_NAME[] = "C:\\temp\\FIAPdev";
    char FIAPDEV_BIN_DIRECTORY_NAME[] = "C:\\temp\\FIAPdev\\bin";
    char FIAPDEV_LIB_DIRECTORY_NAME[] = "C:\\temp\\FIAPdev\\lib";
    HANDLE hArquivo;
    char ArquivoINI[] = "C:\\temp\\FIAPdev\\FIAPdev.ini";
    char DadosINI[] = "[SERVER]\nVersion= 3.7.19\nIP=\t 127.0.0.1\nPort=\t 53173\nTimeOut= 3000\n";
    DWORD dwBytesParaEscrever = (DWORD)strlen(DadosINI);
    DWORD dwBytesEscritos = 0;

    // altera para a localidade padrão do sistema operacional
    // (para poder usar acentuação corretamente)
    setlocale(LC_ALL, "");

    bRetorno = CreateDirectoryA(FIAPDEV_DIRECTORY_NAME, NULL);

    // CreateDirectoryA() = ERRO?
    if (bRetorno == 0)
    {
        // sim, obtém código de erro e ecoa mensagem de erro
        dwErro = GetLastError();
        if (dwErro != ERROR_ALREADY_EXISTS)
        {
            printf("Não foi possível criar o diretório FIAPdev: (%d)).\n", (int)(dwErro));
            return (FIAPDEV_DIR_ERROR);
        }
    }

    bRetorno = CreateDirectoryA(FIAPDEV_BIN_DIRECTORY_NAME, NULL);

    // CreateDirectoryA() = ERRO?
    if (bRetorno == 0)
    {
        // sim, obtém código de erro e ecoa mensagem de erro
        dwErro = GetLastError();
        if (dwErro != ERROR_ALREADY_EXISTS)
        {
            printf("Não foi possível criar o diretório FIAPdev\\bin: (%d)).\n", (int)(dwErro));
            return (FIAPDEV_BIN_DIR_ERROR);
        }
    }

    bRetorno = CreateDirectoryA(FIAPDEV_LIB_DIRECTORY_NAME, NULL);

    // CreateDirectoryA() = ERRO?
    if (bRetorno == 0)
    {
        // sim, obtém código de erro e ecoa mensagem de erro
        dwErro = GetLastError();
        if (dwErro != ERROR_ALREADY_EXISTS)
        {
            printf("Não foi possível criar o diretório FIAPdev\\lib: (%d)).\n", (int)(dwErro));
            return (FIAPDEV_LIB_DIR_ERROR);
        }
    }

    hArquivo = CreateFileA((LPCSTR)&ArquivoINI,     // lpFileName: nome do arquivo .ini
                           GENERIC_WRITE,           // dwDesiredAccess: cria o arquivo para escrita
                           0,                       // dwShareMode: arquivo não compartilhado
                           NULL,                    // lpSecurityAttributes: atributos de segurança padrão
                           CREATE_ALWAYS,           // dwCreationDisposition: cria sempre um novo arquivo
                           FILE_ATTRIBUTE_NORMAL,   // dwFlagsAndAttributes: arquivo normal
                           NULL);                   // hTemplateFile: sem estensão de atribulos

    // CreateFileA() = ERRO?
    if (hArquivo == INVALID_HANDLE_VALUE)
    {
        // sim, obtém código de erro e ecoa mensagem de erro
        dwErro = GetLastError();
        if (dwErro !=  ERROR_ALREADY_EXISTS)
        {
            printf("Não foi possível criar o arquivo FIAPdev.INI: (%d)).\n", (int)(dwErro));
            return (FIAPDEV_INI_CREATE_ERROR);
        }
    }

    bRetorno = WriteFile(hArquivo,              // hFile: handle para o arquivo FIAPdev.INI
                         DadosINI,              // lpBuffer: buffer contendo os dados a serem escritos
                         dwBytesParaEscrever,   // nNumberOfBytesToWrite: números de bytes a serem escritos
                         &dwBytesEscritos,      // lpNumberOfBytesWritten: número de bytes escritos (efetivamente)
                         NULL);                 // lpOverlapped: no overlapped structure

    // WriteFile() = ERRO?
    if (bRetorno == FALSE)
    {
        // sim, obtém código de erro e ecoa mensagem de erro
        dwErro = GetLastError();
        printf("Não foi possível escrever no arquivo FIAPdev.INI: (%d)).\n", (int)(dwErro));
        // libera handle para o arquivo FIAPdev.INI
        CloseHandle(hArquivo);
        return (FIAPDEV_INI_WRITE_ERROR);
    }

    // libera handle para o arquivo FIAPdev.INI
    CloseHandle(hArquivo);
    printf("FIAPdev instalado com SUCESSO!\n");
    return(FIAPDEV_OK);
}
