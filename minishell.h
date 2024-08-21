/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinsecho <jinsecho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 17:27:52 by jinsecho          #+#    #+#             */
/*   Updated: 2024/08/12 20:34:35 by jinsecho         ###   ########.fr       */
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

typedef struct	s_term
{
	struct termios	current_term;
	struct termios	alter_term;
}	t_term;
typedef	struct	s_cmd
{
	char			*line;
	char			**line_split;
	// char			**pipe_split;
	struct s_term	term;
}	t_cmd;

void	ms_term_set(t_cmd *cmd);
void	ms_term_reset(t_cmd *cmd);

#endif
