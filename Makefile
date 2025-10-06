name = miniRT

sources =	main.c \
			clean_utils.c \
			common_utils.c \
			common_utils_2.c \
			common_utils_3.c \
			create_vectors.c \
			create_vectors_utils.c \
			cylinder_caps.c \
			cylinder_intersection_utils.c \
			cylinder_shadow_rays_utils.c \
			error_handlers.c \
			fill_ambiance.c \
			fill_camera.c \
			fill_cylinder.c \
			fill_light.c \
			fill_plane.c \
			fill_sphere.c \
			fill_utils.c \
			init_hit_record.c \
			init_objects.c \
			intersections.c \
			math_operations.c \
			light_management.c \
			light_management_utils.c \
			parser.c \
			plane_intersection_utils.c \
			populate_structs.c \
			put_pixel_utils.c \
			sphere_intersection_utils.c \
			validate_array_utils.c \
			vector_operations.c \
			vector_operations_2.c \
			key_events.c \

CC = cc

INCLUDES = -I./ -I$(LIBFT_DIR) -I$(MLX_DIR)

CFLAGS = -Wall -Wextra -Werror -g ${INCLUDES}

GREEN = \033[0;32m
RESET = \033[0m

#flag -L indica o folder -l indica (lib)+<biblioteca sem prefixo lib e sem extensão>
LINKS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

LIBFT_DIR = includes/libft/
LIBFT = $(LIBFT_DIR)libft.a

MLX_DIR = includes/minilibx-linux/
MLX = $(MLX_DIR)libmlx.a
CFLAGS_MINILIBX = $(filter-out -Werror,$(CFLAGS))

OBJ = $(sources:.c=.o)

all: $(LIBFT) $(MLX) $(name)

$(LIBFT):
	@$(MAKE) -s -C $(LIBFT_DIR) CFLAGS="$(CFLAGS)"
	@echo "$(GREEN)Libft compiled!$(RESET)"

$(MLX): $(MLX_DIR)
	@echo "$(GREEN)Compiling Minilibx...$(RESET)"
	@$(MAKE) -s -C $(MLX_DIR)
	@echo "$(GREEN)Minilibx compiled!$(RESET)"

$(name): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) -o $(name) $(OBJ) $(LINKS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

valgrind: $(name)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(name) scenes/scene_1.rt

clean:
	rm -f $(OBJ)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
#@$(MAKE) -s -C $(MLX_DIR) clean

fclean: clean
	rm -f $(name)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
#@$(MAKE) -s -C $(MLX_DIR) fclean

re: fclean all

.PHONY: all clean fclean re valgrind