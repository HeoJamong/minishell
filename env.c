#include "minishell.h"

void    print_env(char **env)
{
    int i;

    i = 0;
    while(env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}

char    **set_env(char **envp)  // 먼저 이 함수 호출해서 환경변수를 다 복사함
{
    char    **new;
    int     i;

    i = 0;
    while (envp[i])
        i++;
    new = malloc(sizeof(char *) * (i + 1));
    if (new == NULL)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        new[i] = ft_strdup(envp[i]);
        i++;
    }
    new[i] = NULL;
    return (new);
}

int check_env(char *str, char *env)
{
    int i;

    i = 0;
    // 저장하려고 하는 환경변수와 현재 저장 된 환경변수를 비교하여 이미 존재하는 환경 변수인지 확인
    while (str[i] && env[i] && (str[i] == env[i]) && (str[i] != '=') && (env[i] != '='))
        i++;
    if (i == 0)
        return (0);
    if ((str[i] == '=') && (env[i] == '=') || (env[i] == '=') || str[i] == 0)
        return (1); //같은 이름의 환경 변수가 존재할 때
    return (0); //존재 하지 않는 환경 변수일 경우
}

void    new_export(char *str, char **new, int index)
{
    new[index] = ft_strdup(str);
    new[index + 1] = NULL;
}

int ft_export(char *str, char ***env)
{
    int i;
    char    **new;

    if (str[0] == '=' || str[0] == '\0' || ft_isalnum(str[0])) // 이거 뭐 _랑 더 있는데 추가해야함
        return (0);
    i = 0;
    while((*env)[i])
    {
        if (check_env(str, (*env)[i]))
        {
            //이미 존재하는 이름의 환경변수라면 기존에 할당 된 값을 프리하고 새로 할당
            free((*env)[i]);
            (*env)[i] = ft_strdup(str);
            return (1);
        }
        i++;
    }
    new = malloc(sizeof(char *) * (i + 2)); //하나 더 추가해줘야 하니 공간 2개 증가
    if (new == NULL)
        exit(1);
    i = 0;
    while (env[i]) //무지성 복사후 초기화 (아니 그냥 연결리스트할껄;;;;;)
    {
        new[i] = ft_strdup((*env)[i]);
        free((*env)[i]);
        i++;
    }
    new_export(str, new ,i);
    free(*env);
    *env = new;
    return (1);
}