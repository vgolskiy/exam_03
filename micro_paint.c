#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef	struct	s_b
{
	int		w;
	int		h;
	char	ch;
}				t_b;

typedef struct	s_z
{
	char		t;
	float		w;
	float		h;
	float		x;
	float		y;
	char		ch;
}				t_z;

static void	init_back(t_b *b)
{
	b->w = 0;
	b->h = 0;
	b->ch = 0;
}

static void	clear_zone(t_z *z)
{
	z->t = 0;
	z->x = 0;
	z->y = 0;
	z->w = 0;
	z->h = 0;
	z->ch = 0;
}

static int	ft_strlen(char *s)
{
	int		i;

	i = 0;
	if (s)
		while (s[i])
			i++;
	return (i);
}

static void	*ft_calloc(int len, int size)
{
	int		i;
	char	*s;
	void	*res;

	if (!(res = malloc(len * size)))
		return (00);
	s = (char *)res;
	i = -1;
	while (++i < (len * size))
		s[i] = '\0';
	return (res);
}

static void	clear_all(FILE *f, char	*pic)
{
	if (f)
		fclose(f);
	if (pic)
	{
		free(pic);
		pic = 00;
	}
}

static int	ft_error(char *s, FILE *f, char *pic)
{
	clear_all(f, pic);
	write(1, s, ft_strlen(s));
	return (1);
}

static int	verify_back(t_b *b)
{
	if (((b->w > 0) && (b->w <= 300))
	&& ((b->h > 0) && (b->h <= 300)))
		return (1);
	return (0);
}

static int	verify_zone(t_z *z)
{
	if ((z->w > 0.0) && (z->h > 0.0)
	&& ((z->t == 'r') || (z->t == 'R')))
		return (1);
	return (0);
}

static char	*get_back(FILE *f, t_b *b)
{
	int		i;
	char	*pic;

	if (fscanf(f, "%d %d %c\n", &b->w, &b->h, &b->ch) != 3)
		return (00);
	if (!verify_back(b))
		return (00);
	if (!(pic = (char *)ft_calloc(b->w * b->h + 1, sizeof(char))))
		return (00);
	i = -1;
	while (++i < (b->w * b->h))
		pic[i] = b->ch;
	return (pic);
}

static int	in_zone(float x, float y, t_z *z)
{
	if ((x >= z->x) && (x <= z->x + z->w)
	&& (y >= z->y) && (y <= z->y + z->h))
	{
		if ((x - z->x < 1.0) || (z->x + z->w - x < 1.0)
		|| (y - z->y < 1.0) || (z->y + z->h - y < 1.0))
			return (2);
		return (1);
	}
	return (0);
}

static void	draw_zone(char **pic, t_z *z, t_b *b)
{
	int		i;
	int		j;
	int		mark;

	i = -1;
	while (++i < b->w)
	{
		j = -1;
		while (++j < b->h)
		{
			mark = in_zone(i, j, z);
			if (((z->t == 'r') && (mark == 2))
			|| ((z->t == 'R') && mark))
				(*pic)[j * b->w + i] = z->ch;
		}
	}
}

static int	draw_zones(FILE *f, char **pic, t_b *b)
{
	t_z	z;
	int	mark;

	clear_zone(&z);
	while ((mark = fscanf(f, "%c %f %f %f %f %c\n", &z.t, &z.x, &z.y, &z.w, &z.h, &z.ch)) == 6)
	{
		if (!verify_zone(&z))
			return (0);
		draw_zone(pic, &z, b);
	}
	if (mark != -1)
		return (0);
	return (1);
}

static void	put_pic(char *pic, t_b *b)
{
	int	j;

	j = -1;
	while (++j < b->h)
	{
		write(1, pic + (j * b->w), b->w);
		write(1, "\n", 1);
	}
}

int			main(int argc, char **argv)
{
	t_b		b;
	char	*pic;
	FILE	*f;

	f = 00;
	pic = 00;
	if (argc != 2)
		return (ft_error("Error: argument\n", f, pic));
	if (!(f = fopen(argv[1], "r")))
		return (ft_error("Error: Operation file corrupted\n", f, pic));
	init_back(&b);
	if (!(pic = get_back(f, &b)))
		return (ft_error("Error: Operation file corrupted\n", f, pic));
	if (!draw_zones(f, &pic, &b))
		return (ft_error("Error: Operation file corrupted\n", f, pic));
	put_pic(pic, &b);
	clear_all(f, pic);
	return (0);
}
