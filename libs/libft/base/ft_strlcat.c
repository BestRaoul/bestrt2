
#include "libft.h"

size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t dstsize)
{
	size_t	i;
	size_t	d_l;
	size_t	s_l;

	d_l = ft_strlen(dst);
	s_l = ft_strlen(src);
	if (dstsize <= d_l)
		return (s_l + dstsize);
	i = 0;
	while (d_l + i < dstsize - 1 && src[i])
	{	
		dst[d_l + i] = src[i];
		i++;
	}
	dst[d_l + i] = '\0';
	return (d_l + s_l);
}
