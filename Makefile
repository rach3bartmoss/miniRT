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

CC		= cc

MAN_CFLAGS	= -Wall -Wextra -Werror -g
BON_CFLAGS	= -Wall -Wextra -Werror -g $(PTHREAD)
PTHREAD		= -pthread

# ──────────────────────────────────────────────────────────────────────────────
#  MANDATORY
# ──────────────────────────────────────────────────────────────────────────────

NAME		= miniRT
MAN_DIR		= mandatory
MAN_SRCS_DIR	= $(MAN_DIR)/srcs
MAN_BUILD	= $(MAN_DIR)/build

MAN_SRCS = \
	$(MAN_SRCS_DIR)/utils/error_handlers.c \
	$(MAN_SRCS_DIR)/utils/common_utils_3.c \
	$(MAN_SRCS_DIR)/utils/common_utils.c \
	$(MAN_SRCS_DIR)/utils/common_utils_2.c \
	$(MAN_SRCS_DIR)/utils/init_hit_record.c \
	$(MAN_SRCS_DIR)/utils/clean_utils.c \
	$(MAN_SRCS_DIR)/utils/init_objects.c \
	$(MAN_SRCS_DIR)/parsing/fill_camera.c \
	$(MAN_SRCS_DIR)/parsing/fill_light.c \
	$(MAN_SRCS_DIR)/parsing/populate_structs.c \
	$(MAN_SRCS_DIR)/parsing/fill_cylinder.c \
	$(MAN_SRCS_DIR)/parsing/fill_ambiance.c \
	$(MAN_SRCS_DIR)/parsing/fill_plane.c \
	$(MAN_SRCS_DIR)/parsing/fill_sphere.c \
	$(MAN_SRCS_DIR)/parsing/validate_array_utils.c \
	$(MAN_SRCS_DIR)/parsing/parser.c \
	$(MAN_SRCS_DIR)/parsing/fill_utils.c \
	$(MAN_SRCS_DIR)/objects/cylinder_intersection_utils.c \
	$(MAN_SRCS_DIR)/objects/plane_intersection.c \
	$(MAN_SRCS_DIR)/objects/sphere_render.c \
	$(MAN_SRCS_DIR)/objects/cylinder_caps.c \
	$(MAN_SRCS_DIR)/objects/sphere_intersection_utils.c \
	$(MAN_SRCS_DIR)/objects/plane_intersection_utils.c \
	$(MAN_SRCS_DIR)/objects/cylinder_shadow_rays_utils.c \
	$(MAN_SRCS_DIR)/objects/cylinder_caps_utils.c \
	$(MAN_SRCS_DIR)/math/create_vectors_utils.c \
	$(MAN_SRCS_DIR)/math/vector_operations_2.c \
	$(MAN_SRCS_DIR)/math/vector_operations_3.c \
	$(MAN_SRCS_DIR)/math/create_vectors.c \
	$(MAN_SRCS_DIR)/math/math_operations.c \
	$(MAN_SRCS_DIR)/math/vector_operations.c \
	$(MAN_SRCS_DIR)/hooks/key_events.c \
	$(MAN_SRCS_DIR)/render/light_management_utils.c \
	$(MAN_SRCS_DIR)/render/light_management.c \
	$(MAN_SRCS_DIR)/render/put_pixel_utils.c \
	$(MAN_SRCS_DIR)/render/render_loop.c \
	$(MAN_SRCS_DIR)/main.c

MAN_OBJS = $(MAN_SRCS:$(MAN_SRCS_DIR)/%.c=$(MAN_BUILD)/%.o)

MAN_LIBFT_DIR	= $(MAN_DIR)/includes/libft
MAN_LIBFT_A		= $(MAN_LIBFT_DIR)/libft.a
MAN_MLX_DIR		= $(MAN_DIR)/includes/minilibx-linux
MAN_MLX_A		= $(MAN_MLX_DIR)/libmlx.a

MAN_INCLUDES	= -I$(MAN_DIR)/includes -I$(MAN_LIBFT_DIR) -I$(MAN_MLX_DIR)
MAN_LIBS		= -L$(MAN_LIBFT_DIR) -lft -L$(MAN_MLX_DIR) -lmlx -lXext -lX11 -lm -lz

# ──────────────────────────────────────────────────────────────────────────────
#  BONUS
# ──────────────────────────────────────────────────────────────────────────────

NAME_BONUS		= miniRT_bonus
BON_DIR			= bonus
BON_SRCS_DIR	= $(BON_DIR)/srcs
BON_BUILD		= $(BON_DIR)/build

BON_SRCS = \
	$(BON_SRCS_DIR)/bonus/apply_checkerboard.c \
	$(BON_SRCS_DIR)/bonus/apply_checkerboard_helper.c \
	$(BON_SRCS_DIR)/bonus/click_event_bonus.c \
	$(BON_SRCS_DIR)/bonus/apply_texture_color.c \
	$(BON_SRCS_DIR)/bonus/apply_bump_mapping.c \
	$(BON_SRCS_DIR)/bonus/plane_texture_color.c \
	$(BON_SRCS_DIR)/bonus/input_textures.c \
	$(BON_SRCS_DIR)/bonus/textures_utils.c \
	$(BON_SRCS_DIR)/bonus/textures_assign.c \
	$(BON_SRCS_DIR)/bonus/load_textures.c \
	$(BON_SRCS_DIR)/utils/error_handlers.c \
	$(BON_SRCS_DIR)/utils/common_utils.c \
	$(BON_SRCS_DIR)/utils/common_utils_2.c \
	$(BON_SRCS_DIR)/utils/common_utils_3.c \
	$(BON_SRCS_DIR)/utils/common_utils_4.c \
	$(BON_SRCS_DIR)/utils/init_hit_record.c \
	$(BON_SRCS_DIR)/utils/clean_utils.c \
	$(BON_SRCS_DIR)/utils/init_objects.c \
	$(BON_SRCS_DIR)/parsing/fill_camera.c \
	$(BON_SRCS_DIR)/parsing/fill_light.c \
	$(BON_SRCS_DIR)/parsing/populate_structs.c \
	$(BON_SRCS_DIR)/parsing/fill_cylinder.c \
	$(BON_SRCS_DIR)/parsing/fill_ambiance.c \
	$(BON_SRCS_DIR)/parsing/fill_plane.c \
	$(BON_SRCS_DIR)/parsing/fill_sphere.c \
	$(BON_SRCS_DIR)/parsing/fill_paraboloid.c \
	$(BON_SRCS_DIR)/parsing/validate_array_utils.c \
	$(BON_SRCS_DIR)/parsing/parser.c \
	$(BON_SRCS_DIR)/parsing/fill_utils.c \
	$(BON_SRCS_DIR)/objects/cylinder_intersection_utils.c \
	$(BON_SRCS_DIR)/objects/plane_intersection.c \
	$(BON_SRCS_DIR)/objects/sphere_render.c \
	$(BON_SRCS_DIR)/objects/cylinder_caps.c \
	$(BON_SRCS_DIR)/objects/sphere_intersection_utils.c \
	$(BON_SRCS_DIR)/objects/plane_intersection_utils.c \
	$(BON_SRCS_DIR)/objects/cylinder_shadow_rays_utils.c \
	$(BON_SRCS_DIR)/objects/cylinder_caps_utils.c \
	$(BON_SRCS_DIR)/objects/paraboloid_calc.c \
	$(BON_SRCS_DIR)/objects/paraboloid_init.c \
	$(BON_SRCS_DIR)/objects/paraboloid_intersection.c \
	$(BON_SRCS_DIR)/math/create_vectors_utils.c \
	$(BON_SRCS_DIR)/math/vector_operations_2.c \
	$(BON_SRCS_DIR)/math/vector_operations_3.c \
	$(BON_SRCS_DIR)/math/create_vectors.c \
	$(BON_SRCS_DIR)/math/math_operations.c \
	$(BON_SRCS_DIR)/math/vector_operations.c \
	$(BON_SRCS_DIR)/hooks/key_events.c \
	$(BON_SRCS_DIR)/hooks/signal_handler.c \
	$(BON_SRCS_DIR)/render/light_management_utils.c \
	$(BON_SRCS_DIR)/render/light_management.c \
	$(BON_SRCS_DIR)/render/light_phong.c \
	$(BON_SRCS_DIR)/render/put_pixel_utils.c \
	$(BON_SRCS_DIR)/render/render_loop.c \
	$(BON_SRCS_DIR)/render/render_thread.c \
	$(BON_SRCS_DIR)/render/shading_utils.c \
	$(BON_SRCS_DIR)/main.c

BON_OBJS = $(BON_SRCS:$(BON_SRCS_DIR)/%.c=$(BON_BUILD)/%.o)

BON_LIBFT_DIR	= $(BON_DIR)/includes/libft
BON_LIBFT_A		= $(BON_LIBFT_DIR)/libft.a
BON_MLX_DIR		= $(BON_DIR)/includes/minilibx-linux
BON_MLX_A		= $(BON_MLX_DIR)/libmlx.a

BON_INCLUDES	= -I$(BON_DIR)/includes -I$(BON_LIBFT_DIR) -I$(BON_MLX_DIR)
BON_LIBS		= -L$(BON_LIBFT_DIR) -lft -L$(BON_MLX_DIR) -lmlx -lXext -lX11 -lm -lz $(PTHREAD)

VAL_RULES = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

all: $(MAN_LIBFT_A) $(MAN_MLX_A) $(NAME)

bonus: $(BON_LIBFT_A) $(BON_MLX_A) $(NAME_BONUS)

# ──────────────────────────────────────────────────────────────────────────────
#  MANDATORY COMPILATION
# ──────────────────────────────────────────────────────────────────────────────

$(MAN_BUILD)/%.o: $(MAN_SRCS_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling$(RESET) → $<"
	@$(CC) $(MAN_CFLAGS) $(MAN_INCLUDES) -c $< -o $@

$(MAN_LIBFT_A):
	@echo "$(MAGENTA)Building libft [mandatory]...$(RESET)"
	@$(MAKE) -s -C $(MAN_LIBFT_DIR)

$(MAN_MLX_A):
	@echo "$(MAGENTA)Building MinilibX [mandatory]...$(RESET)"
	@$(MAKE) -s -C $(MAN_MLX_DIR) 2>/dev/null

$(NAME): $(MAN_OBJS)
	@echo "$(YELLOW)Linking objects...$(RESET)"
	@$(CC) $(MAN_CFLAGS) -o $(NAME) $(MAN_OBJS) $(MAN_LIBS)
	@echo "$(GREEN)✔ Build complete: $(BOLD)$(NAME)$(RESET)"

# ──────────────────────────────────────────────────────────────────────────────
#  BONUS COMPILATION
# ──────────────────────────────────────────────────────────────────────────────

$(BON_BUILD)/%.o: $(BON_SRCS_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling$(RESET) → $<"
	@$(CC) $(BON_CFLAGS) $(BON_INCLUDES) -c $< -o $@

$(BON_LIBFT_A):
	@echo "$(MAGENTA)Building libft [bonus]...$(RESET)"
	@$(MAKE) -s -C $(BON_LIBFT_DIR)

$(BON_MLX_A):
	@echo "$(MAGENTA)Building MinilibX [bonus]...$(RESET)"
	@$(MAKE) -s -C $(BON_MLX_DIR) 2>/dev/null

$(NAME_BONUS): $(BON_OBJS)
	@echo "$(YELLOW)Linking bonus objects...$(RESET)"
	@$(CC) $(BON_CFLAGS) -o $(NAME_BONUS) $(BON_OBJS) $(BON_LIBS)
	@echo "$(GREEN)✔ Build complete: $(BOLD)$(NAME_BONUS)$(RESET)"

# ──────────────────────────────────────────────────────────────────────────────
#  UTILITY RULES
# ──────────────────────────────────────────────────────────────────────────────

valgrind: all
	@echo "$(BLUE)Running with Valgrind...$(RESET)"
	@valgrind $(VAL_RULES) ./$(NAME) scenes/scene_1.rt

valgrind_bonus: bonus
	@echo "$(BLUE)Running with Valgrind [bonus]...$(RESET)"
	@valgrind $(VAL_RULES) ./$(NAME_BONUS) scenes/scene_1.rt

clean:
	@echo "$(RED)Cleaning mandatory objects...$(RESET)"
	@rm -rf $(MAN_BUILD)
	@$(MAKE) clean -s -C $(MAN_LIBFT_DIR)
	@$(MAKE) clean -s -C $(MAN_MLX_DIR)
	@echo "$(RED)Cleaning bonus objects...$(RESET)"
	@rm -rf $(BON_BUILD)
	@$(MAKE) clean -s -C $(BON_LIBFT_DIR)
	@$(MAKE) clean -s -C $(BON_MLX_DIR)

fclean: clean
	@echo "$(RED)Removing executables...$(RESET)"
	@rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) fclean -s -C $(MAN_LIBFT_DIR)
	@$(MAKE) fclean -s -C $(BON_LIBFT_DIR)

re: fclean all

re_bonus: fclean bonus

.PHONY: all bonus clean fclean re re_bonus valgrind valgrind_bonus