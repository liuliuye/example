#include "application.h"


void warpHandle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h, u16 *r)
{
		s32 x, y, i, j, k = buf_w/2, l = buf_h/2, /*m,*/ n, halfh = buf_h / 2, rcount;
		s32 y_offset=0,uv_offset=0,fally_offset=(buf_h - 1) * buf_w,falluv_offset=(buf_h/2-1) * buf_w;
		for(y = 0,rcount = -1; y < halfh; y += 1)
		{
			if(y < halfh)
				rcount++;
			else
				rcount--;
			if(r)
				i = r[rcount];
			else
				i = rcount;
			for(x = 0; x < buf_w; x++)
			{
				if(x < k)
					n = k - 1 -x;
				else
					n = x;

				if((n > l - i) && (n < buf_w - l + i))
				{
					j = (n+i-l)*k/(k - l + i);

					ybuf[y_offset + n]=ybuf[y_offset + j];
					ybuf[fally_offset + n]=ybuf[fally_offset + j];

					if((0==(y%2)))
					{
						s32 set_n,set_j;
						set_n=n&(~0x1);
						set_j=j&(~0x1);

						uvbuf[uv_offset+set_n]=uvbuf[uv_offset + set_j];
						uvbuf[uv_offset+set_n+1]=uvbuf[uv_offset + set_j+1];
						uvbuf[falluv_offset+set_n]=uvbuf[falluv_offset + set_j];
						uvbuf[falluv_offset+set_n+1]=uvbuf[falluv_offset + set_j+1];
					}
				}
			}
			y_offset+=buf_w;
			fally_offset-=buf_w;
			if(1==(y%2))
			{
				uv_offset+=buf_w;
				falluv_offset-=buf_w;
			}
		}
	}

void trilateral_16_window(u8* ybuf,u16 *uvbuf,u16 w,u16 h)
{
	INT32S /*x, y, z,*/ i, j, k, l, resolution = h * w,reverse = ((h >> 1) - 1) * w, starty = w*80*(h/180), startx = 160*(w/320);
	u16 a = w/4, b = w/2, c = w*3/4, d = w/4 - 1, e = w/2 - 1, f = w*3/4 - 1, g = w - 1;
	
	for(j = 0, k = 0, l = (w/20); j <= reverse; j += w,k += 55){
		if(k >= 100){
			k -= 100;
			l++;
		}
		for(i = 0; i < l; i++){
			*(ybuf + reverse - j + g - i) = *(ybuf + reverse - j + f - i) = *(ybuf + reverse - j + e - i) = *(ybuf + reverse - j + d - i) = *(ybuf + j + i + c) = *(ybuf + j + i + b) = *(ybuf + j + i + a) = *(ybuf + j + i) = *(ybuf + j + starty + i + startx);
		}
		if(0 == ((j/w)%2)){
			for(i = 0; i < l; i+=2){
				*(uvbuf + ((reverse - j) >> 2) + ((g - i) >> 1)) = *(uvbuf + ((reverse - j) >> 2) + ((f - i) >> 1)) = *(uvbuf + ((reverse - j) >> 2) + ((e - i) >> 1)) = *(uvbuf + ((reverse - j) >> 2) + ((d - i) >> 1)) = 
					*(uvbuf + (j >> 2) + ((i + c) >> 1)) = *(uvbuf + (j >> 2) + ((i + b) >> 1)) = *(uvbuf + (j >> 2) + ((i + a) >> 1)) = *(uvbuf + (j >> 2) + (i >> 1)) = *(uvbuf + ((j + starty) >> 2) + ((i + startx) >> 1));
			}
		}
	}

	resolution = (resolution >> 1);
	for(j = resolution; j < (resolution << 1); j += w){
		for(i = 0; i < w; i++){
			*(ybuf + j + i) = *(ybuf + j + i - resolution);
		}
		if(0 == ((j/w)%2)){
			for(i = 0; i < w; i+=2){
				*(uvbuf + (j >> 2) + (i >> 1)) = *(uvbuf + ((j - resolution) >> 2) + (i >> 1));
			}
		}
	}
}

INT32U window_stream_flag = 0;
void stream_10_window(u8* ybuf,u16 *uvbuf,u16 w,u16 h)	
{
	INT32S x, y, i, j, k, l, m, n, a, b, c, d, flag, l_is_even;
	if(h == 180)
		l_is_even = w / 4;
	else
		l_is_even = 0;
	k = w / 4;
	l = h / 4;
	m = l * w;
	n = (h - l) * w;
	flag = k -1 - window_stream_flag % k;
	
	for(y = 0,j = l * w; y < m; y += w,j += w << 1){
		
		for(x = flag,i = 2 * k; x < flag + k; x++,i += 2){
			a = y + x;
			b = a + k;
			c = b + k;
			d = c + k;
			if(d > y + w - 1)
				d = d - w;
			ybuf[n + a] = ybuf[n + b] = ybuf[n + c] = ybuf[n + d] =ybuf[a] = ybuf[b] = ybuf[c] = ybuf[d] = ybuf[j + i];
		}
		if(0 == ((y/w)%2)){
			for(x = flag,i = 2 * k; x < flag + k; x+=2,i += 4){
				a = (y >> 2) + (x >> 1);
				b = a + (k >> 1);
				c = b + (k >> 1);
				d = c + (k >> 1);
				if(d > (y >> 2) + ((w - 1) >> 1))
					d = d - (w >> 1);
				uvbuf[(n >> 2) + a] = uvbuf[(n >> 2) + b] = uvbuf[(n >> 2) + c] = uvbuf[(n >> 2) + d] =uvbuf[a] = uvbuf[b] = uvbuf[c] = uvbuf[d] = uvbuf[(j >> 2) + (i >> 1) - l_is_even];
			}
		}
	}
	k = w / 2;
	l = h / 2;
	m = (l + h / 4) * w;
	flag = window_stream_flag % k;
	for(j = h / 4 * w; y < m; y += w,j += w){
		
		for(x = flag,i = w/2; x < flag + k; x++,i++){
			ybuf[x + y] = ybuf[j + i];
		}
		for(x = flag; x < flag + k; x++){
			a = x + k;
			if(a > w - 1)
				a -= w;
			ybuf[a + y] = ybuf[x + y];
		}
		if(0 == ((y/w)%2)){
			for(x = flag,i = w/2; x < flag + k; x+=2,i+=2){
				uvbuf[(y >> 2) + (x >> 1)] = uvbuf[(j >> 2) + (i >> 1)];
			}
			for(x = flag; x < flag + k; x+=2){
				a = ((x + k) >> 1);
				if(a > ((w - 1) >> 1))
					a -= (w >> 1);
				uvbuf[a + (y >> 2)] = uvbuf[(y >> 2) + (x >> 1)];
			}
		}
	}
	//window_stream_flag++;
}


void rismatic_multiwindow(u8* ybuf,u16 *uvbuf,u16 w,u16 h)  //
{
		INT32S a, b, c, x, y, /*i,*/ j, m, n, resolution = h * w,halfh = h / 2 * w;
		j = h*3/4 * w;
		for(y = h/2 * w,b = w/2 - 1,c = h/2 * w,m = w/2 - 1,n = w/2; y < j; y += w,b--,c -= w,m --,n ++){
			for(x = m,a = c; x < n; x++,a += w){
				ybuf[resolution - a + (w - 2) - b] = ybuf[a + b] = ybuf[resolution - y + (w - 2) - x] = ybuf[y + x];
			}
			if(0 == ((y/w)%2)){
				for(x = m,a = c; x < n; x+=2,a += w << 1){
					uvbuf[((resolution - a) >> 2) + (((w - 2) - b) >> 1)] = uvbuf[(a >> 2) + (b >> 1)] = uvbuf[((resolution - y) >> 2) + (((w - 2) - x) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
		}
		j = h * w;
		for(; y < j; y += w,b--,c += w,m ++,n --){
			for(x = m,a = c; x < n; x++,a += w){
				ybuf[resolution - a + (w - 2) - b] = ybuf[a + b] = ybuf[resolution - y + (w - 2) - x] = ybuf[y + x];
			}
			if(0 == ((y/w)%2)){
				for(x = m,a = c; x < n; x+=2,a += w << 1){
					uvbuf[((resolution - a) >> 2) + (((w - 2) - b) >> 1)] = uvbuf[(a >> 2) + (b >> 1)] = uvbuf[((resolution - y) >> 2) + (((w - 2) - x) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
		}
		n = 0;
		for(y = h/9 * w; y < h*8/9 * w; y += w){
			if(y < halfh)
				n++;
			else
				n--;
			for(x = 0; x < n; x++){
				ybuf[y + x] = ybuf[y + h + x];
				ybuf[y + (w - 2) - x] = ybuf[y + (w - 2) - h - x];
			}
			ybuf[y + (w - 1)] = ybuf[y + (w - 1) - h];//Swallow the edge (w - 2)
			if(0 == ((y/w)%2)){
				for(x = 0; x < n; x+=2){
					uvbuf[(y >> 2) + (x >> 1)] = uvbuf[(y >> 2) + ((h + x) >> 1)];
					uvbuf[(y >> 2) + (((w - 2) - x) >> 1)] = uvbuf[(y >> 2) + (((w - 2) - h - x) >> 1)];
				}
			}
		}
		
		for(y = 0,m = w/2 - 1,n = w/2; y < halfh; y += w, m--, n++){
			for(x = m; x < n; x++){
				a = x - (h >> 1);
				b = x + (h >> 1);
				if(a >= 0){
					ybuf[y + halfh + a] = ybuf[y + x];
					ybuf[resolution - y - halfh + a] = ybuf[resolution - y + x];
				}
				if(b < w){
					ybuf[y + halfh + b] = ybuf[y + x];
					ybuf[resolution - y - halfh + b] = ybuf[resolution - y + x];
				}
			}
			if(0 == ((y/w)%2)){
				for(x = m; x < n; x+=2){
					a = x - (h >> 1);
					b = x + (h >> 1);
					if(a >= 0){
						uvbuf[((y + halfh) >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
						uvbuf[((resolution - y - halfh) >> 2) + (a >> 1)] = uvbuf[((resolution - y) >> 2) + (x >> 1)];
					}
					if(b < w){
						uvbuf[((y + halfh) >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
						uvbuf[((resolution - y - halfh) >> 2) + (b >> 1)] = uvbuf[((resolution - y) >> 2) + (x >> 1)];
					}
				}
			}
		}
}

void hexagon_multiwindow(u8* ybuf,u16 *uvbuf,u16 w,u16 h)
{
	INT32S a, b, c, d, x, y, /*i,*/ j, p1 = (h/180)*27, p2 = p1*2, p3 = p1*3 & (~0x1), p4 = p1*4, m, n, resolution = h * w, evenX, evenY;
	j = (h - p4)/2 * w + p1 * w;
	for(y = (h - p4)/2 * w,m = (w/2 - 1),n = w/2; y < j; y += w,m -= 2,n += 2){
		evenY = ((y/w)%2) ? 0 : 1;
		for(x = m; x < n; x++){
			evenX = x & 1 ? 0 : 1;
			ybuf[y + p3*w + x + p2] = ybuf[y + x];
			ybuf[y + p3*w + x - p2] = ybuf[y + x];
			if(evenX && evenY){
				uvbuf[((y + p3*w) >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				uvbuf[((y + p3*w) >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
			}
			a = x + p4;
			if(a < w){
				ybuf[y + a] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				a += p2;
				if(a < w){
					ybuf[y + p3*w + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y + p3*w) >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
			b = x - p4;
			if(b >= 0){
				ybuf[y + b] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				b -= p2;
				if(b >= 0){
					ybuf[y + p3*w + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y + p3*w) >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
		}
	}



	j += p2 * w;
	for(; y < j; y += w){
		evenY = ((y/w)%2) ? 0 : 1;
		c = y + p3*w;
		d = y - p3*w;
		for(x = m; x < n; x++){
			evenX = x & 1 ? 0 : 1;
			
			if(c < resolution){
				ybuf[c + x + p2] = ybuf[y + x];
				ybuf[c + x - p2] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(c >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					uvbuf[(c >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
			if(d >= 0){
				ybuf[d + x + p2] = ybuf[y + x];
				ybuf[d + x - p2] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(d >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					uvbuf[(d >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}

			a = x + p4;
			if(a < w){
				ybuf[y + a] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
			a = x + p4 + p2;
			if(a < w){
				if(c < resolution){
					ybuf[c + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(c >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
				if(d >= 0){
					ybuf[d + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(d >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}

			
			b = x - p4;
			if(b >= 0){
				ybuf[y + b] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
			}
			b = x - p4 - p2;
			if(b >= 0){
				if(c < resolution){
					ybuf[c + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(c >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
				if(d >= 0){
					ybuf[d + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[(d >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
		}
	}

	j += p1 * w;
	for(; y < j; y += w,m += 2,n -= 2){
		evenY = ((y/w)%2) ? 0 : 1;
		for(x = m; x < n; x++){
			evenX = x & 1 ? 0 : 1;
			ybuf[y - p3*w + x + p2] = ybuf[y + x];
			ybuf[y - p3*w + x - p2] = ybuf[y + x];
			if(evenX && evenY){
				uvbuf[((y - p3*w) >> 2) + ((x + p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				uvbuf[((y - p3*w) >> 2) + ((x - p2) >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
			}
			a = x + p4;
			if(a < w){
				ybuf[y + a] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				a += p2;
				if(a < w){
					ybuf[y - p3*w + a] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y - p3*w) >> 2) + (a >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
			b = x - p4;
			if(b >= 0){
				ybuf[y + b] = ybuf[y + x];
				if(evenX && evenY){
					uvbuf[(y >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
				}
				b -= p2;
				if(b >= 0){
					ybuf[y - p3*w + b] = ybuf[y + x];
					if(evenX && evenY){
						uvbuf[((y - p3*w) >> 2) + (b >> 1)] = uvbuf[(y >> 2) + (x >> 1)];
					}
				}
			}
		}
	}
}


