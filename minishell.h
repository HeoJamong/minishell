/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jheo <jheo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:27:52 by jinsecho          #+#    #+#             */
/*   Updated: 2024/09/14 19:57:26 by jheo             ###   ########.fr       */
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
	struct termios	alter_term;
}	t_term;
typedef	struct	s_cmd
{
	int				line_i;
	char			*line;
	char			**line_split;
	char			**envp;
	struct s_term	term;
}	t_cmd;
typedef struct	s_env_var
{
	char	*env_tmp;
	char	*str_tmp;
	int		cnt;
	int		len;
	int		n;
}	t_env_var;

void		ms_term_set(t_cmd *cmd);
void		ms_term_reset(t_cmd *cmd);
char		*ft_envchr(char *env, char *str);
char		*ft_realloc(char *ptr, int size);
char		**set_env(char **envp);
int			ft_export(char *str, t_cmd *cmd);
void		print_env(char **env);
int			ft_unset(char *str, t_cmd *cmd);
long long	ft_atol(const char *string);
#endif
