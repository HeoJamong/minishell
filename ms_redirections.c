#include "minishell.h"

int input_redirect(char *str)
{
	int	input_file;

	input_file = open(str, O_RDONLY);
	if (input_file < 0)
	{
		printf("그런 파일이나 디렉터리가 없습니다.\n");
		return (0);
	}
	if (dup2(input_file, 0) == -1)
	{
		printf("dup error\n");
		return (0);
	}
	close(0);
	return (1);
}

int	output_redirect(char *str)
{
	int	out_file;

	out_file = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (out_file < 0)
	{
		printf("뭔가 잘못 됐음\n");
		return (0);
	}
	if (dup2(out_file, 1) == -1)
	{
		printf("dop error\n");
		return (0);
	}
	close(1);
	return (1);
}