#include "../cub3d.h"

int char_in_str(char cr, char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (str[i] == cr)
			return 1;
	}
	return 0;
}

int numOfTextures(int side, int stepY, int stepX)
{
	if (side == 1)
    {
      if (stepY == -1)
        return 0;//запад
      else
        color = 0x7FFFD4; //восток
    }
    else
    {
      if (stepX == -1)
        return 0;// север DarkBlue
      else
        return 1; //юг Lime
    }
}

int it_is_not_wall(t_map* map, double row, double col)
{
	printf("%d %d %c \n", (int)row, (int)col, map->field[(int)row] [(int)col]);
	if(map->field[(int)row] [(int)col] != '0')
		return 0;
	else if(map->field[(int)(row + 0.11)] [(int)(col + 0.11)] != '0')
		return 0;
	else if(map->field[(int)(row + 0.11)] [(int)(col)] != 48)
		return 0;
	else if(map->field[(int)(row)] [(int)(col + 0.11)] != 48)
		return 0;
	else if(map->field[(int)(row - 0.11)] [(int)(col - 0.11)] != 48)
		return 0;
	else if(map->field[(int)(row - 0.11)] [(int)(col)] != 48)
		return 0;
	else if(map->field[(int)(row)] [(int)(col - 0.11)] != 48)
		return 0;
	return (1);
}
void	change_player_position(t_map *map, double row, double col)
{
	// int	row2;
	// int	col2;

	// row2 = (int)(map->lodev.posX + 2 * (row - map->lodev.posX));
	// col2 = (int)(map->lodev.posY + 2 * (col - map->lodev.posY));
	printf("%f %f\n", row, col);
	// printf("%d %d \n", row2, col2);
	// 	// && map->field[(int)map->lodev.posX][col2] != '1' && map->field[col2][(int)map->lodev.posX] != '1')
	if (it_is_not_wall(map, row, col))
	{
		map->lodev.posX = row;
		map->lodev.posY = col;
	}
	else if (it_is_not_wall(map, map->lodev.posX, col))
	{
		map->lodev.posX = row;
	}
	else if (it_is_not_wall(map, row, map->lodev.posY))
	{
		map->lodev.posY = col;
		
	}
}

void move_forward(t_map *map, int flag, double *x, double *y)
{
	*x += flag * map->lodev.dirX * (map->lodev.moveSpeed);
	*y += flag * map->lodev.dirY * (map->lodev.moveSpeed);	
}


void move_side(t_map *map, int flag, double *x, double *y)
{
	*y -= flag * map->lodev.dirX * (map->lodev.moveSpeed);
	*x += flag * map->lodev.dirY * (map->lodev.moveSpeed);
}

int	ft_map_print(t_map *map)
{
	printf("Hello\n");
	
	for(int x = 0; x < map->lodev.w; x++)
    {
		//printf("x= %d\n", x);
		double cameraX = 2 * x / (double)map->lodev.w - 1; //x-coordinate in camera space
		double rayDirX = map->lodev.dirX + map->lodev.planeX * cameraX;
		double rayDirY =  map->lodev.dirY + map->lodev.planeY * cameraX;

		int mapX = (int)(map->lodev.posX);
		int mapY = (int)(map->lodev.posY);

		double sideDistX;
		double sideDistY;

		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);
		double perpWallDist;

		int stepX;
		int stepY;

		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?

		if(rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (map->lodev.posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - map->lodev.posX) * deltaDistX;
		}
		if(rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (map->lodev.posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - map->lodev.posY) * deltaDistY;
		}

		while (hit == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if(sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
		//printf("hit= %d\n", hit);

			if(map->field[mapX][mapY] == '1')
				hit = 1; //-------------------------------------------???????????
		}
		//Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
		if(side == 0)
			perpWallDist = (mapX -  map->lodev.posX + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY -  map->lodev.posY + (1 - stepY) / 2) / rayDirY;

		int lineHeight = (int)(map->lodev.h / perpWallDist);

		int drawStart = -lineHeight / 2 + map->lodev.h / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + map->lodev.h / 2;
		if(drawEnd >= map->lodev.h)
			drawEnd = map->lodev.h - 1;
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------

		double wallX; //где именно была пробита стена
		if (side == 0)
			wallX = map->lodev.posY + perpWallDist * rayDirY;
		else
			wallX = map->lodev.posX + perpWallDist * rayDirX;
		wallX = wallX - (int)(wallX);
		// координата x на текстуре
		//printf("WallX= %f\n", wallX);
		int texX = (int)(wallX * (double)(map->text[0].sprites_width));
		if(side == 0 && rayDirX > 0)
			texX = map->text[0].sprites_width - texX - 1;
		if(side == 1 && rayDirY < 0)
			texX = map->text[0].sprites_width - texX - 1;
		
		double step = 1.0 *  map->text[0].sprites_height / lineHeight;
		double texPos = (drawStart -  map->lodev.h / 2 + lineHeight / 2) * step;		

		//------------------------------------------------------------------------
				// while (i < map->spec.r.y)
				// {
				// 	if (i >= drawStart && i <= drawEnd)
				// 	{
				// 		my_mlx_pixel_put(&map->grap, x, i, color);
				// 	}
				// 	else if (i < drawStart)
				// 		my_mlx_pixel_put(&map->grap, x, i, Blue);
				// 	//my_mlx_pixel_put(&map->grap, x, i, color);
				// 	i++;
				// }
				//Вот так рисуется текстуру вместо цвета

	// if (side == 1)
    // {
    //   if (stepY == -1)
    //     color = 0x000000; //запад
    //   else
    //     color = 0x7FFFD4; //восток
    // }
    // else
    // {
    //   if (stepX == -1)
    //     color = 0x00008B; // север DarkBlue
    //   else
    //     color = 0x00FF00; //юг Lime
    // }
		for (int w = 0; w < map->spec.r.y; w++)
		{
			int numOfTextures = NumOfText();
			if (w >= drawStart && w <= drawEnd)
			{
				int texY = (int)texPos & (map->text[0].sprites_height - 1);
				texPos += step;
				my_mlx_pixel_put(&map->grap, x, w, map->text[0].data[texY * map->text[0].sprites_height + texX]);
			}
			else if (w < drawStart)
				my_mlx_pixel_put(&map->grap, x, w, map->c);
			else if (w > drawEnd)
				my_mlx_pixel_put(&map->grap, x, w, map->f);
		}
	}

	printf("map->lodev.dirX= %f ", map->lodev.dirX);
	printf("map->lodev.dirY= %f\n", map->lodev.dirY);

	printf("map->lodev.planeX= %f ", map->lodev.planeX);
	printf("map->lodev.planeY= %f\n", map->lodev.planeY);

	mlx_put_image_to_window(map->grap.mlx, map->grap.win, map->grap.img, 0, 0);

	return (0);
}

/*
printf("map->spec.r.x= %d\n", map->spec.r.x);
printf("map->spec.r.y= %d\n", map->spec.r.y);
printf("line= %d\n", map->line); // по вертикали 
printf("column= %d\n", map->column); // по горизонтали
printf("cell_1= %d\n", cell_1);
printf("cell_2= %d\n", cell_2);
printf("cell= %d\n", cell);
printf("map->lodev.posX= %d\n", (int)map->lodev.posX);
printf("map->lodev.posY= %d\n", (int)map->lodev.posY);
*/
void	ft_mini_map(t_map *map)
{
	if (map->mini_map.size == 0)
		return ;
	int cell = min(map->spec.r.x / map->column / map->mini_map.size,
					map->spec.r.y /  map->line / map->mini_map.size);

	t_coord	start;
	t_coord size;
	size.x = cell;
	size.y = cell;

	for (int i = 0; i < map->line; i++)
	{
		for (int j = 0; j < map->column; j++)
		{
			start.y = i * cell;
			start.x = j * cell;

			if (map->mini_map.mode == 0)
			{
				if (map->field[i][j] == '1')
					square_print(map, start, size, map->mini_map.color_wall);
				else if (map->field[i][j] == '0')
					square_print(map, start, size, map->mini_map.color_empty);
			}
			else if (map->mini_map.mode == 1)
			{
				if (map->field[i][j] == '1')
					square_print(map, start, size, map->mini_map.color_wall);
			}
			else if (map->mini_map.mode == 2)
			{
				if (map->field[i][j] == '0')
					square_print(map, start, size, map->mini_map.color_empty);
			}
		}
	}
	start.x = (map->lodev.posY - 0.5) * cell + cell/4;
	start.y = (map->lodev.posX - 0.5) * cell + cell/4;
	size.x = cell/2;
	size.y = cell/2;
	square_print(map, start, size, map->mini_map.color_player);
	//-------------------------------------------------------------------------------
}

int	key_hook_press(int keycode, t_map *map)
{
	mlx_destroy_image(map->grap.mlx, map->grap.img);
	map->grap.img = mlx_new_image(map->grap.mlx, map->spec.r.x, map->spec.r.y);
	map->grap.addr = mlx_get_data_addr(map->grap.img, &map->grap.bits_per_pixel, &map->grap.line_length, &map->grap.endian); 

	double testPosX = map->lodev.posX;
	double testPosY = map->lodev.posY;

	if (keycode == Esc )
	{
		printf("Выход через esc\n");
		mlx_destroy_window(map->grap.mlx, map->grap.win);
		exit (0);
	}
	else if (keycode == Up && map->button.up == 0) //printf("Вверх нажато\n");
		map->button.up = 1;
	else if (keycode == Down && map->button.down == 0) //printf("Вниз нажато\n");
		map->button.down = 1;
	else if (keycode == Right && map->button.right == 0) //printf("Вправо нажато\n");
		map->button.right = 1;
	else if (keycode == Left && map->button.left == 0) //printf("Влево нажато\n");
		map->button.left = 1;
	else if (keycode == Turn_right && map->button.turn_right == 0) //printf("Поворот вправо нажато\n");
		map->button.turn_right = 1;
	else if (keycode == Turn_left && map->button.turn_left == 0) //printf("Поворот влево нажато\n");
		map->button.turn_left = 1;
	else if (keycode == M)
	{
		if (map->mini_map.size == 4)
			map->mini_map.size = 0;
		else
			map->mini_map.size++;
		printf("Режим миникарты size\n");
	}
	else if (keycode == F)
	{
		if (map->mini_map.mode == 2)
			map->mini_map.mode = 0;
		else
			map->mini_map.mode++;
		printf("Режим миникарты mode\n");
	}
	else
		printf("Неизвестная клавиша %d\n", keycode);

	// движение вперёд/назад
	if (map->button.up == 1 && map->button.down == 0)
		move_forward(map, 1, &testPosX, &testPosY);
	else if (map->button.down == 1 && map->button.up == 0)
		move_forward(map, -1, &testPosX, &testPosY);
	// движение вправо/влево
	if (map->button.right == 1 && map->button.left == 0)
		move_side(map, 1, &testPosX, &testPosY);
	else if (map->button.left == 1 && map->button.right == 0)
		move_side(map, -1, &testPosX, &testPosY);
	// поворот вправо/влево
	if (map->button.turn_right == 1 && map->button.turn_left == 0)
	{
		double oldDirX = map->lodev.dirX;
		map->lodev.dirX = map->lodev.dirX * cos(-map->lodev.rotSpeed) - map->lodev.dirY * sin(-map->lodev.rotSpeed);
		map->lodev.dirY = oldDirX * sin(-map->lodev.rotSpeed) + map->lodev.dirY * cos(-map->lodev.rotSpeed);
		double oldPlaneX = map->lodev.planeX;
		map->lodev.planeX = map->lodev.planeX * cos(-map->lodev.rotSpeed) - map->lodev.planeY * sin(-map->lodev.rotSpeed);
		map->lodev.planeY = oldPlaneX * sin(-map->lodev.rotSpeed) + map->lodev.planeY * cos(-map->lodev.rotSpeed);
	}
	else if (map->button.turn_left == 1 && map->button.turn_right == 0)
	{
		double oldDirX = map->lodev.dirX;
		map->lodev.dirX = map->lodev.dirX * cos(map->lodev.rotSpeed) - map->lodev.dirY * sin(map->lodev.rotSpeed);
		map->lodev.dirY = oldDirX * sin(map->lodev.rotSpeed) + map->lodev.dirY * cos(map->lodev.rotSpeed);
		double oldPlaneX = map->lodev.planeX;
		map->lodev.planeX = map->lodev.planeX * cos(map->lodev.rotSpeed) - map->lodev.planeY * sin(map->lodev.rotSpeed);
		map->lodev.planeY = oldPlaneX * sin(map->lodev.rotSpeed) + map->lodev.planeY * cos(map->lodev.rotSpeed);
	}

	change_player_position(map, testPosX, testPosY);
	//-Менять-----------
	ft_map_print(map); // старая
	ft_mini_map(map);
	//------------------
	return (1);
}

int	key_hook_repress(int keycode, t_map *map)
{
	if (keycode == Up && map->button.up == 1) //printf("Вверх отжато\n");
		map->button.up = 0;
	else if (keycode == Down && map->button.down == 1) //printf("Вниз отжато\n");
		map->button.down = 0;
	else if (keycode == Right && map->button.right == 1) //printf("Вправо отжато\n");
		map->button.right = 0;
	else if (keycode == Left && map->button.left == 1) //printf("Влево отжато\n");
		map->button.left = 0;
	else if (keycode == Turn_right && map->button.turn_right == 1) //printf("Поворот вправо отжато\n");
		map->button.turn_right = 0;
	else if (keycode == Turn_left && map->button.turn_left == 1) //printf("Поворот влево отжато\n");
		map->button.turn_left = 0;

	return (1);
}

int key_hook_close(int keycode, t_map *map)
{
	printf("Выход через крестик\n");
	exit (0);
}

int		main_graphics(t_map *map)
{
	if (init_grap(map) == 0)
		return (0);

	//-----------
	init_text(map);
	init_mini_map(map);
	//-----------
	ft_map_print(map);
	ft_mini_map(map);
	mlx_hook(map->grap.win, 2, 1L << 2, key_hook_press, map);
	mlx_hook(map->grap.win, 3, 1L << 3, key_hook_repress, map); 
	mlx_hook(map->grap.win, 17, 1L << 2, key_hook_close, map);
	mlx_loop(map->grap.mlx);


	return (1);
}
