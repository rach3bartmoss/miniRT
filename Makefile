# **************************************************************************** #
#                                   COLORS                                     #
# **************************************************************************** #

RESET   = \033[0m
BOLD    = \033[1m
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
CYAN    = \033[36m
MAGENTA = \033[35m

# **************************************************************************** #
#                                   PROJECT                                    #
# **************************************************************************** #

NAME = miniRT
CC = cc

SRCS = 	srcs/bonus/apply_checkerboard.c \
		srcs/bonus/apply_checkerboard_helper.c \
		srcs/bonus/click_event_bonus.c \
		srcs/utils/error_handlers.c \
		srcs/utils/common_utils_3.c \
		srcs/utils/common_utils.c \
		srcs/utils/common_utils_2.c \
		srcs/utils/common_utils_4.c \
		srcs/utils/init_hit_record.c \
		srcs/utils/clean_utils.c \
		srcs/utils/init_objects.c \
		srcs/parsing/fill_camera.c \
		srcs/parsing/fill_light.c \
		srcs/parsing/populate_structs.c \
		srcs/parsing/fill_cylinder.c \
		srcs/parsing/fill_ambiance.c \
		srcs/parsing/fill_plane.c \
		srcs/parsing/fill_sphere.c \
		srcs/parsing/fill_paraboloid.c \
		srcs/parsing/validate_array_utils.c \
		srcs/parsing/parser.c \
		srcs/parsing/fill_utils.c \
		srcs/objects/cylinder_intersection_utils.c \
		srcs/objects/plane_intersection.c \
		srcs/objects/sphere_render.c \
		srcs/objects/cylinder_caps.c \
		srcs/objects/sphere_intersection_utils.c \
		srcs/objects/plane_intersection_utils.c \
		srcs/objects/cylinder_shadow_rays_utils.c \
		srcs/objects/cylinder_caps_utils.c \
		srcs/math/create_vectors_utils.c \
		srcs/math/vector_operations_2.c \
		srcs/math/vector_operations_3.c \
		srcs/math/create_vectors.c \
		srcs/math/math_operations.c \
		srcs/math/vector_operations.c \
		srcs/hooks/key_events.c \
		srcs/render/light_management_utils.c \
		srcs/render/light_management.c \
		srcs/render/light_phong.c \
		srcs/render/put_pixel_utils.c \
		srcs/render/render_loop.c \
		srcs/render/shading_utils.c \
		srcs/main.c

BUILD_DIR = build
OBJS = $(SRCS:srcs/%.c=$(BUILD_DIR)/%.o)

LIBFT_DIR = includes/libft
LIBFT_A = $(LIBFT_DIR)/libft.a

MLX_DIR = includes/minilibx-linux
MLX_A = $(MLX_DIR)/libmlx.a

INCLUDES = -Iincludes -I$(LIBFT_DIR) -I$(MLX_DIR)
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES) $(PTHREAD)

PTHREAD = -pthread

LIBS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz $(PTHREAD)

VAL_RULES = --leak-check=full --show-leak-kinds=all --track-origins=yes

# **************************************************************************** #
#                                   RULES                                      #
# **************************************************************************** #

all: $(LIBFT_A) $(MLX_A) $(NAME)

$(BUILD_DIR)/%.o: srcs/%.c Makefile
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling$(RESET) → $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	@echo "$(MAGENTA)Building libft...$(RESET)"
	@$(MAKE) -s -C $(LIBFT_DIR)

$(MLX_A):
	@echo "$(MAGENTA)Building MinilibX...$(RESET)"
	@$(MAKE) -s -C $(MLX_DIR)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking objects...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)
	@echo "$(GREEN)✔ Build complete: $(BOLD)$(NAME)$(RESET)"

valgrind: all
	@echo "$(BLUE)Running with Valgrind...$(RESET)"
	@valgrind $(VAL_RULES) ./$(NAME) scenes/scene_1.rt

clean:
	@echo "$(RED)Cleaning objects...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@$(MAKE) clean -C $(LIBFT_DIR)
	@$(MAKE) clean -C $(MLX_DIR)

fclean: clean
	@echo "$(RED)Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re valgrind
