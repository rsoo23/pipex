/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsoo <rsoo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 21:57:12 by rsoo              #+#    #+#             */
/*   Updated: 2023/06/03 21:57:12 by rsoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	execute_cmd(t_info *info)
{
	int		i;
	char	*cmd_path;

	i = -1;
	while (info->path_list[++i])
	{
		cmd_path = ft_strjoin(info->path_list[i], "/");
		cmd_path = ft_strjoin(cmd_path, info->cmds[info->cmd_index][0]);
		printf("cmdpath:%s, ind: %d, cmd_ind: %d\n\n", cmd_path, i, info->cmd_index);
		if (access(cmd_path, X_OK) == 0)
		{
			if (execve(cmd_path, info->cmds[info->cmd_index], NULL) == -1)
			{
				free(cmd_path);
				free_and_exit(info);
			}
		}
	}
}

/*
dup2(old, new);
- lets say the file has 3 as its fd, it duplicates the fd so that
  the new one also represents the fd of the file
- below: fd_in and 0 represents the file fd
- redirect STDIN to the file fd
*/

static void	child_process(t_info *info)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		free_and_exit(info);
	info->pid = fork();
	if (info->pid == -1)
		free_and_exit(info);
	if (info->pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execute_cmd(info);
		info->cmd_index++;
	}
	else if (info->pid > 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		waitpid(info->pid, NULL, 0);
	}
}

/*
fork():
child: opens the write end of pipe
parent: opens the read end of pipe and waits for child to end

piping:
1. connect the pipes first / do the redirection first

	initial: STDIN --> cmd 1 --> STDOUT
	result:   fdin --> cmd 1 --> pipefd[1]

2. execute cmd while parent waits
*/

void	piping(t_info *info, int ac)
{
	if (dup2(info->fd_in, STDIN_FILENO) == -1)
		free_and_exit(info);
	while (ac-- > 4)
	{
		child_process(info);
		printf("ac: %d\n", ac);
	}
	if (dup2(info->fd_out, STDOUT_FILENO) == -1)
		free_and_exit(info);
	execute_cmd(info);
}
