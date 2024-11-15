#include "minishell.h"

int input_redirect(char *file_name)
{
	int	input_file;

	input_file = open(file_name, O_RDONLY);
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

int	output_redirect(char *file_name)
{
	int	out_file;

	out_file = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (out_file < 0)
	{
		printf("뭔가 잘못 됐음\n");
		return (0);
	}
	if (dup2(out_file, 1) == -1)
	{
		printf("dup error\n");
		return (0);
	}
	close(1);
	return (1);
}

int	continuing_redirect(char *file_name)
{
	char continuing_file;

	continuing_file = open(file_name, O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0666);
	if (continuing_file < 0)
	{
		printf("뭔가 잘못 됐음\n");
		return (0);
	}
		if (dup2(continuing_file, 1) == -1)
	{
		printf("dup error\n");
		return (0);
	}
	close(1);
	return (1);
}

int	here_doc(char *last_word, t_cmd *cmd)
{

	char	*str;
	char	*line;

	line = ft_strdup("");
	while(1)
	{
		str = readline("> ");

		if (ft_strlen(str) == ft_strlen(last_word) \
		&& !ft_strncmp(str, last_word, ft_strlen(last_word)))
		{
			free(str);
			break;
		}
		line = ft_strjoin(line, str);
		line = ft_strjoin(line, "\n");
		free(str);
	}
	cmd->rdr.line = line;
	return (1);
}

int	here_doc_pipe(char	*here_line, t_cmd *cmd)
{
	int	fd;

	fd = open("tmp.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	while (*here_line)
	{
		write(fd, here_line, 1);
		here_line++;
	}
	close(fd);
	fd = open("tmp.txt", O_RDONLY);
	cmd->rdr.fd = fd;
	return(1);
}
