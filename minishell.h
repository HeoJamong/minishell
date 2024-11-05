/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:27:52 by jinsecho          #+#    #+#             */
/*   Updated: 2024/11/04 18:40:41 by jheo             ###   ########.fr       */
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
# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34

typedef struct	s_term
{
	struct termios	current_term;
	struct termios	change_term;
}	t_term;
typedef struct	s_plst
{
	char			**pipe_split;
	struct s_plst	*next;
}	t_plst;
typedef	struct	s_cmd
{
	int				process_status; // "$?"를 구현하기위해 만든 변수
	int				line_i;
	int				pipe_true;
	char			*line;
	char			**line_split;
	char			**envp;
	struct s_term	term;
	struct s_plst	*pipe_lst;
}	t_cmd;
typedef struct	s_env_var
{
	char	*env_tmp;
	char	*str_tmp;
	int		cnt;
	int		len;
	int		n;
}	t_env_var;

// util_func
char		*ft_realloc(char *ptr, int size);
void		line_split_free(t_cmd *cmd);
long long	ft_atol(const char *string);

// term_set
void		ms_term_set(t_cmd *cmd);
void		ms_term_reset(t_cmd *cmd);

// env_func
char		*ft_envchr(char *env, char *str);
char		**set_env(char **envp);
void		print_env(char **env);
void		ms_line_replace_env(t_cmd *cmd, char **ptr, char *line);

int			ft_export(char *str, t_cmd *cmd);
int			ft_unset(char *str, t_cmd *cmd);

// tokenizer
char	*ms_line_tokenizing_quote(t_cmd *cmd, char *line, int *i);
char	*ms_line_tokenizing_str(t_cmd *cmd, char *line, int *i);
void	ms_line_tokenizer(t_cmd *cmd, char *line);
void	line_token_quote(t_cmd *cmd, char *line, int *line_i, int *i);
void	line_token_redirect(t_cmd *cmd, char *line, int *line_i, int *i);
void	line_token_str(t_cmd *cmd, char *line, int *line_i, int *i);

// pipe_linked_lst
t_plst	*ms_lstnew(void);
t_plst	*ms_lstlast(t_plst *lst);
void	ms_lstadd_back(t_plst **lst, t_plst *new);

//redirections
int	input_redirect(char *str);
int	output_redirect(char *str);
#endif
