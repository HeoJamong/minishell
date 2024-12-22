/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:27:52 by jinsecho          #+#    #+#             */
/*   Updated: 2024/12/22 17:42:40 by jheo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <termios.h>
# include <sys/wait.h>
# include <dirent.h>
# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34

typedef struct s_pipefd
{
	int	*c;
	int	*p;
}	t_pipefd;

typedef struct s_fd
{
	int	input;
	int	output;
}	t_fd;
typedef struct s_rdr
{
	char	*line;
	int		fd;
}	t_rdr;
typedef struct s_term
{
	struct termios	current_term;
	struct termios	change_term;
}	t_term;
typedef struct s_plst
{
	char			**pipe_split;
	int				*heredoc_fd;
	int				heredoc_true;
	int				file_fd;
	int				rdr_true;
	int				rdr_input_file;
	int				ca_cnt;
	struct s_rdr	rdr;
	struct s_plst	*prev;
	struct s_plst	*next;
}	t_plst;
typedef struct s_sts
{
	int	process_status;
	int	pipe_true;
}	t_sts;
typedef struct s_tmp
{
	int	i;
	int	k;
}	t_tmp;
typedef struct s_cmd
{
	int				pipe_cnt;
	int				line_i;
	char			*line;
	char			**line_split;
	char			**envp;
	struct s_tmp	fd;
	struct s_sts	sts;
	struct s_term	term;
	struct s_plst	*pipe_lst;
}	t_cmd;
typedef struct s_env_var
{
	char	*env_tmp;
	char	*str_tmp;
	int		cnt;
	int		len;
	int		n;
}	t_env_var;

// util_func
char		*ft_realloc(char *ptr, int size);
void		ft_line_split_free(char **tmp);
long long	ft_atoll(const char *string);

// term_set
void		ms_term_set(t_cmd *cmd, int i);
void		ms_term_reset(t_cmd *cmd, int i);

// env_func
char		*ft_envchr(char *env, char *str);
char		**set_env(char **envp);
void		ms_line_replace_env(t_cmd *cmd, char **ptr, char *line);

int			ft_export(char *str, t_cmd *cmd);
int			ft_unset(char *str, t_cmd *cmd);

// tokenizer
char		*ms_line_tokenizing_quote(t_cmd *cmd, char *line, int *i);
char		*ms_line_tokenizing_str(t_cmd *cmd, char *line, int *i);
void		ms_line_tokenizer(t_cmd *cmd, char *line);
void		line_token_quote(t_cmd *cmd, char *line, int *line_i, int *i);
void		line_token_redirect(t_cmd *cmd, char *line, int *line_i, int *i);
void		line_token_str(t_cmd *cmd, char *line, int *line_i, int *i);
int			ms_line_pipe_split(t_cmd *cmd);

// pipe_linked_lst
t_plst		*ms_lstnew(void);
t_plst		*ms_lstlast(t_plst *lst);
void		ms_lstadd_back(t_plst **lst, t_plst *new);

// redirections
int			ms_rdr_input_true(t_cmd *cmd, int i);
int			ms_rdr_output_true(t_cmd *cmd, int i);
void		rdr_input_var_init(int *i, t_plst *tmp);
void		rdr_input_fd_init(t_plst *tmp);
void		ms_error_print(int i);
int			rdr_heredoc(t_cmd *cmd, t_plst *tmp, int *i, int k);
int			rdr_file_input(t_cmd *cmd, t_plst *tmp, int *i, int k);
int			rdr_file_output(t_plst *tmp, int *i, int k);
int			rdr_file_output_append(t_plst *tmp, int *i, int k);

// pipe_exec
int			ms_exec(t_cmd *cmd);
void		ms_line_parsing_exec(t_cmd *cmd);
void		cmd_pipe_exec(t_cmd *cmd, t_plst *tmp);
void		cmd_path_cat_exec(t_cmd *cmd, t_plst *tmp);
void		pipe_exec(t_cmd *cmd, t_plst *tmp, int **fds, int *pid_idx);
int			pipe_exec_begin(t_cmd *cmd, t_plst *tmp, int *fd, int **fds);
int			pipe_exec_end(t_cmd *cmd, t_plst *tmp, int *fd, int **fds);
int			pipe_exec_middle(t_cmd *cmd, t_plst *tmp, t_pipefd *fd, int **fds);

// builtin_func
int			ms_builtin_func(t_cmd *cmd, t_plst *tmp);
int			start_exit(int	*ca_cnt, t_plst *lst_tmp, t_cmd *cmd);
int			exit_num_isdigit(char *num, int *flag);
int			max_exit_num_check(char num, int i);
int			min_exit_num_check(char num, int i);
int			print_export(char **env);
int			check_ispossible_export(int c);
int			print_env(char **env);
int			check_env(char *str, char *env);
int			find_char_index(char *str, char c);
int			start_cd(t_cmd *cmd, t_plst *lst_tmp, int ca_cnt);
int			cd_tild_cntroller(t_plst *lst_tmp, t_cmd *cmd);
void		count_ca_cnt(t_plst *l);

#endif
