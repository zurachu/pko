const int sin_table[91] = {
  0,  18,  35,  52,  70,  87, 105, 122, 139, 156, 174, 191, 208, 225, 242,
259, 276, 293, 309, 326, 342, 358, 375, 391, 407, 423, 438, 454, 470, 485,
500, 515, 530, 545, 559, 574, 588, 602, 616, 629, 643, 656, 669, 682, 695,
707, 719, 731, 743, 755, 766, 777, 788, 800, 809, 819, 829, 839, 848, 857,
866, 875, 883, 891, 899, 906, 914, 921, 927, 934, 940, 946, 951, 956, 961,
966, 970, 974, 978, 982, 985, 988, 990, 993, 995, 996, 998, 999, 999,
1000, 1000 };

const int arctan_table[21] = {
 0,  3,  6,  9, 11, 14, 17, 19, 22, 24,
27, 29, 31, 33, 35, 37, 39, 41, 42, 44,
45 };

int Get_Sin( int index )
{
	while( index < -90 ) { index += 360; }
	while( index > 270 ) { index -= 360; }
	
	if( index > 90 ) { index = 180 - index; }
	if( index < 0 ) { return( -sin_table[-index] ); }
	return( sin_table[index] );
}


int Get_Cos( int index )
{
	return( Get_Sin( index + 90 ) );
}


int Get_ArcTan( int x, int y )
{
	int i = 0;
	
	if( x == 0 && y == 0 )	{ return( 180 ); }
	if( x < 0 && y < 0 ){
		x = -x;
		y = -y;
		i = 180;
	}
	if( x < 0 ){
		i = -x;
		x = y;
		y = i;
		i =  90;
	}
	if( y < 0 ){
		i = x;
		x = -y;
		y = i;
		i = -90;
	}
	
	if( y <= x )	{ return( i + arctan_table[y*20/x] ); }
	else			{ return( i + 90 - arctan_table[x*20/y] ); }
}

