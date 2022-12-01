int CNewAlgDoc::AR1N(cmplx *R, cmplx *A, int N, float *W)
{
//========================================================
struct cmplx *U, *V, *RV1;
float S, SS_re, SS_im;
float EPS;
int i,j, l, M, NU, NV, IP, index,index1;

	M=N;
	IP=0;

// form-e  wh. matricy
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			index=i*N+j;
			if(j-i > 0)  { A[index].re=R[j-i].re; A[index].im=-R[j-i].im; }
			else  { A[index].re=R[i-j].re; A[index].im=R[i-j].im; }
		}
/*
char buf[200], ch[50];
for(i=0;i<200;i++) buf[i]=0;
for(i=0;i<N;i++)	{ sprintf(ch,"(%f ; %f)  ", R[i].re,R[i].im); strcat(buf,ch); }
	AfxMessageBox(buf);

for(i=0;i<200;i++) buf[i]=0;
for(i=0;i<N;i++)
{
	for(j=0;j<N;j++)
	{ 
		sprintf(ch,"(%2.4f ; %2.4f)  ", A[i*N+j].re,A[i*N+j].im);
		strcat(buf,ch);
	}
		strcat(buf,"\n");
}

	AfxMessageBox(buf);
*/
/*
//=========== Test =========
N=2;
M=3;
A[0].re=2.8f; A[0].im=-0.4f;
A[1].re=1.6f; A[1].im=0;
A[2].re=3.6f; A[2].im=-1.2f;
A[3].re=2.4f; A[3].im=-1.8f;
A[4].re=2.0f; A[4].im=0.4f;
A[5].re=2.8f; A[5].im=-4.4f;
//==========================
//*/
/*
N=3;
M=3;
A[0].re=10.f;  A[0].im=0.0f;
A[1].re=-2.f;  A[1].im=0.33f;
A[2].re=0.1f;  A[2].im=5.5f;
A[3].re=-2.f;  A[3].im=-0.33f;
A[4].re=10.f;  A[4].im=0.0f;
A[5].re=-2.f;  A[5].im=0.33f;
A[6].re=0.1f;  A[6].im=-5.5f;
A[7].re=-2.f;  A[7].im=-0.33f;
A[8].re=10.f;  A[8].im=0.0f;
//*/
//==========================

	NU=M;
	NV=N;
int	nmax=N;
	if(M>nmax) nmax=M;

// *******************
	RV1=new struct cmplx[nmax];
	U=new struct cmplx[nmax*nmax];
	V=new struct cmplx[nmax*nmax];

	CSVD (A,M,N,NU,NV,W,U,V);
/*
	C_SVD_Ermit(A,U,N,float(2.4E-16));
	for(i=0;i<N;i++) { W[i]=A[i*N+i].re; }
	for(i=0;i<N;i++)
		for(j=0;j<N;j++) { V[i*N+j].re=U[i*N+j].re; V[i*N+j].im=U[i*N+j].im; }
//*/
/*
char buf[200], ch[50];
for(i=0;i<200;i++) buf[i]=0;
for(i=0;i<N;i++)	{ sprintf(ch,"%f  ", W[i]); strcat(buf,ch); }
	AfxMessageBox(buf);

for(i=0;i<200;i++) buf[i]=0;
for(i=0;i<N;i++)
{
	for(j=0;j<N;j++)
	{ 
		sprintf(ch,"(%2.4f ; %2.4f)  ", V[i*N+j].re,V[i*N+j].im);
		strcat(buf,ch);
	}
		strcat(buf,"\n");
}

	AfxMessageBox(buf);
for(i=0;i<200;i++) buf[i]=0;
for(i=0;i<M;i++)
{
	for(j=0;j<M;j++)
	{ 
		sprintf(ch,"(%2.4f ; %2.4f)  ", U[i*M+j].re,U[i*M+j].im);
		strcat(buf,ch);
	}
		strcat(buf,"\n");
}

	AfxMessageBox(buf);
//*/
// *******************
	EPS=0.000001f;
	for(i=0;i<N;i++) EPS+=W[i];
	EPS=.0001f*EPS/(float)N;
// transponirowanie
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			index=i*N+j;
			index1=j*N+i;
			A[index].re=U[index1].re;
			A[index].im=-U[index1].im;
		}
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			index=i*N+j;
			U[index].re=A[index].re;
			U[index].im=A[index].im;
		}

		for(i=0;i<N;i++)
		{
			S=0;
 			if(W[i]>EPS) S=1.f/W[i];
			W[i]=S;
		}
// sborka  psewdoobratnoj  matricy
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			index=i*N+j;
		 	RV1[j].re=V[index].re*W[j];
		 	RV1[j].im=V[index].im*W[j];
		}
		for(j=0;j<N;j++)
		{
			SS_re=0; SS_im=0;
				for(l=0;l<N;l++)
				{
					index=l*N+j;
					SS_re+=RV1[l].re*U[index].re - RV1[l].im*U[index].im;
					SS_im+=RV1[l].im*U[index].re + RV1[l].re*U[index].im;
				}
		A[i*N+j].re=SS_re;
		A[i*N+j].im=SS_im;
		}
	}
/*char buf[200], ch[50];
for(i=0;i<200;i++) buf[i]=0;
for(i=0;i<N;i++)
{
	for(j=0;j<N;j++)
	{ 
		sprintf(ch,"(%2.4f ; %2.4f)  ", A[i*N+j].re,A[i*N+j].im);
		strcat(buf,ch);
	}
		strcat(buf,"\n");
}
	AfxMessageBox(buf);
*/
	delete [] V;
	delete [] U;
	delete [] RV1;
	return 0;
}

int CNewAlgDoc::CSVD(struct cmplx *A, int M, int N, int NU, int NV, float *S, struct cmplx *U, struct cmplx *V)
{
int i,j,k,k1,kk,kc,kc1,l,l1,ll;
int nmax;
int IP=0, NP, N1;	
float Q_re, Q_im, R_re, R_im,Z,W, tmp,tmp1, EPS, CS, SN, F, H;
float X,Y,G;
float *B, *C, *T;
double ETA=1.2E-7,TOL=2.4E-16;
int index, index1;

	nmax=N;
	if(M>nmax) nmax=M;
	B=new float [nmax];
	C=new float [nmax];
	T=new float [nmax];

	for(i=0;i<nmax;i++) { B[i]=0; C[i]=0; T[i]=0; }

	NP=N+IP;
	N1=N+1;
//	ponivenie porqdka
	C[0]=0;
	k=1;
m10:
	k1=k+1;
	
	kc=k-1;
	kc1=k1-1;
//	iskl`~enie |lementow a(I,K),I=K+1,...,M
	Z=0;
	for(i=kc;i<M;i++) { index=i*N+kc; Z+=A[index].re*A[index].re + A[index].im*A[index].im; }
	B[kc]=0;
	if(Z <= TOL) goto m70;
	Z=(float)sqrt(Z);
	B[kc]=Z;
	index=kc*N+kc;
	W=(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
	Q_re=1; Q_im=0;
	if(fabs(W) >= TOL*TOL) { Q_re=A[index].re/W; Q_im=A[index].im/W; }
	A[index].re=Q_re*(Z+W);
	A[index].im=Q_im*(Z+W);
	
	if(k == NP) goto m70;

		for(j=kc1;j<NP;j++)
		{
			Q_re=0; Q_im=0;
				for(i=kc;i<M;i++)
				{
					index=i*N+j;
					index1=i*N+kc;
					Q_re+=A[index].re*A[index1].re + A[index].im*A[index1].im;
					Q_im+=A[index].im*A[index1].re - A[index].re*A[index1].im;
				}
			Q_re/=(Z*(Z+W));
			Q_im/=(Z*(Z+W));

			for(i=kc;i<M;i++)
			{
				index=i*N+j;
				index1=i*N+kc;
				tmp=Q_re*A[index1].re - Q_im*A[index1].im;
				tmp1=Q_im*A[index1].re + Q_re*A[index1].im;
				A[index].re-=tmp;
				A[index].im-=tmp1;
			}
		}
//	preobrazowanie fazy
	index=kc*N+kc;
	tmp=(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
	Q_re=-A[index].re/tmp;
	Q_im=A[index].im/tmp;
	
		for(j=kc1;j<NP;j++)
		{
			index=kc*N+j;
			tmp=Q_re*A[index].re - Q_im*A[index].im;
			tmp1=Q_im*A[index].re + Q_re*A[index].im;
			A[index].re=tmp;
			A[index].im=tmp1;
		}
//	iskl`~enie |lementow a(K,J),J=K+2,...,N
m70:
	if(k == N) goto m140;
	Z=0;
	for(j=kc1;j<N;j++){ index=kc*N+j; Z+=A[index].re*A[index].re + A[index].im*A[index].im; }

	C[kc1]=0;
	if(Z <= TOL) goto m130;
	Z=(float)sqrt(Z);
	C[kc1]=Z;
	index=kc*N+kc1;
	W=(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
	Q_re=1; Q_im=0;
	if(fabs(W ) > TOL*TOL) { Q_re=A[kc*N+kc1].re/W; Q_im=A[kc*N+kc1].im/W; }
	A[index].re=Q_re*(Z+W);
	A[index].im=Q_im*(Z+W);

	for(i=kc1;i<M;i++)
	{
		Q_re=0; Q_im=0;
		for(j=kc1;j<N;j++)
		{
			index=i*N+j;
			index1=kc*N+j;
			Q_re+=A[index].re*A[index1].re + A[index].im*A[index1].im;
			Q_im+=A[index].im*A[index1].re - A[index].re*A[index1].im;
		}
		Q_re/=(Z*(Z+W));
		Q_im/=(Z*(Z+W));

		for(j=kc1;j<N;j++)
		{
			index=i*N+j;
			index1=kc*N+j;	
			tmp=Q_re*A[index1].re - Q_im*A[index1].im;
			tmp1=Q_im*A[index1].re + Q_re*A[index1].im;
			A[index].re-=tmp;
			A[index].im-=tmp1;
		}
	}
	
//	preobrazowanie fazy
	index=kc*N+kc1;
	tmp=(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
	Q_re=-A[index].re/tmp;
	Q_im=A[index].im/tmp;

	for(i=kc1;i<M;i++)
		{
			index=i*N+kc1;
			tmp=Q_re*A[index].re - Q_im*A[index].im;
			tmp1=Q_im*A[index].re + Q_re*A[index].im;
			A[index].re=tmp;
			A[index].im=tmp1;
		}
m130:
	k=k1;
	goto m10;
//	dopusk dlq prenebrevimo malyh |lementow
m140:
	EPS=0;
	for(k=0;k<N;k++)
	{
		S[k]=B[k];
		T[k]=C[k];
		if((S[k]+T[k]) > EPS) EPS=S[k]+T[k];
	}
	EPS=(float)(EPS*ETA);
//	inicializaciq wy~islenij U i V
	if (NU == 0) goto m180;
	
	for(j=0;j<NU;j++)
	{
		for(i=0;i<M;i++)
		{
			U[i*M+j].re=0;
			U[i*M+j].im=0;
		}
		U[j*M+j].re=1;
		U[j*M+j].im=0;
	}
m180:
	if(NV == 0) goto m210;
	for(j=0;j<NV;j++)
	{
		for(i=0;i<N;i++)
		{
			V[i*N+j].re=0;
			V[i*N+j].im=0;
		}
		V[j*N+j].re=1;
		V[j*N+j].im=0;
	}

//	QR-diagonalizaciq
m210:
	for(kk=1;kk<=N;kk++)
	{
		k=N1-kk;
//	prowerka na ras}eplenie
m220:
			for(ll=1;ll<=k;ll++)
			{
				l=k+1-ll;
				if((float)fabs(T[l-1]) <= EPS) goto m290;
				if((float)fabs(S[l-2]) <= EPS) goto m240; // l-1 ???
			}
//	sokra}enie B(L)
m240:
		CS=0;
		SN=1;
		l1=l-1;
		for(i=l;i<=k;i++)
		{
			F=SN*T[i-1];
			T[i-1]=CS*T[i-1];
			if((float)fabs(F) <= EPS) goto m290;
			H=S[i-1];
			W=(float)sqrt(F*F+H*H);
			S[i-1]=W;
			CS=H/W;
			SN=-F/W;
			if(NU == 0) goto m260;
			for(j=0;j<N;j++)
			{
				index=j*M+l1-1;
				index1=j*M+i-1;
				X=U[index].re;
				Y=U[index1].re;
				U[index].re=X*CS+Y*SN;
				U[index].im=0;
				U[index1].re=Y*CS-X*SN;
				U[index1].im=0;
			}
m260:	
			if(NP == N) continue;
				for(j=N1;j<=NP;j++)
				{
					index=(l1-1)*N+j-1;
					index1=(i-1)*N+j-1;
					Q_re=A[index].re;
					Q_im=A[index].im;
					R_re=A[index1].re;
					R_im=A[index1].im;
					A[index].re=Q_re*CS+R_re*SN;
					A[index].im=Q_im*CS+R_im*SN;
					A[index1].re=R_re*CS-Q_re*SN;
					A[index1].im=R_im*CS-Q_im*SN;
				}
		}

//	prowerka shodimosti
m290:
		W=S[k-1];
		if(l == k) goto m360;
//	sdwig na~ala koordinat
		X=S[l-1];
		Y=S[k-2];
		G=T[k-2];
		H=T[k-1];
		F=((Y-W)*(Y+W)+(G-H)*(G+H))/(2*H*Y);
		G=(float)sqrt(F*F+1);
		if(F < 0) G=-G;
		F=((X-W)*(X+W)+(Y/(F+G)-H)*H)/X;
//	QR {ag
		CS=1;
		SN=1;
		l1=l+1;
		for(i=l1;i<=k;i++)
		{
			G=T[i-1];
			Y=S[i-1];
			H=SN*G;
			G=CS*G;
			W=(float)sqrt(H*H+F*F);
			T[i-2]=W;
			CS=F/W;
			SN=H/W;
			F=X*CS+G*SN;
			G=G*CS-X*SN;
			H=Y*SN;
			Y=Y*CS;
			if(NV == 0) goto m310;
				for(j=0;j<N;j++)
				{
					index=j*N+i-1;
					X=V[index-1].re;
					W=V[index].re;
					V[index-1].re=X*CS+W*SN;
					V[index-1].im=0;
					V[index].re=W*CS-X*SN;
					V[index].im=0;
				}

  m310:	
			W=(float)sqrt(H*H+F*F);
			S[i-2]=W;
			CS=F/W;
			SN=H/W;
			F=CS*G+SN*Y;
			X=CS*Y-SN*G;
			if(NU == 0) goto m330;
				for(j=0;j<N;j++)
				{
					index=j*M+i-1;
					Y=U[index-1].re;
					W=U[index].re;
					U[index-1].re=Y*CS+W*SN;
					U[index-1].im=0;
					U[index].re=W*CS-Y*SN;
					U[index].im=0;
				}

m330:	
			if(NP == N) continue;
			for(j=N1;j<=NP;j++)
			{
				index=(i-2)*N+j-1;
				index1=(i-1)*N+j-1;
				Q_re=A[index].re;
				Q_im=A[index].im;
				R_re=A[index1].re;
				R_im=A[index1].im;
				A[index].re=Q_re*CS+R_re*SN;
				A[index].im=Q_im*CS+R_im*SN;
				A[index1].re=R_re*CS-Q_re*SN;
				A[index1].im=R_im*CS-Q_im*SN;
			}
		}
		T[l-1]=0;
		T[k-1]=F;
		S[k-1]=X;
		goto m220;
//	shodimostx
m360:	
		if(W >= 0) continue;
		S[k-1]=-W;
		if(NV == 0) continue;
		for(j=0;j<N;j++)
		{
			index=j*N+k-1;
			V[index].re=-V[index].re;
			V[index].im=-V[index].im;
		}
	}

//	uporqdo~enie singulqrnyh ~isel
		for(k=0;k<N;k++)
		{
		G=-1;
		j=k;
			for(i=k;i<N;i++)
			{
				if(S[i] <= G) continue;
				G=S[i];
				j=i;
			}
		if(j == k) continue;
		S[j]=S[k];
		S[k]=G;
		if(NV == 0) goto m410;
			for(i=0;i<N;i++)
			{
				index=i*N+j;
				index1=i*N+k;
				Q_re=V[index].re;
				Q_im=V[index].im;
				V[index].re=V[index1].re;
				V[index].im=V[index1].im;
			    V[index1].re=Q_re;
			    V[index1].im=Q_im;
			}
m410: 	
		if(NU == 0) goto m430;
			for(i=0;i<N;i++)
			{
				index=i*M+j;
				index1=i*M+k;
				Q_re=U[index].re;
				Q_im=U[index].im;
				U[index].re=U[index1].re;
				U[index].im=U[index1].im;
				U[index1].re=Q_re;
				U[index1].im=Q_im;
			}
m430:	
		if(NP == N) continue;
			for(i=N1-1;i<NP;i++)
			{
				index=j*N+i;
				index1=k*N+i;
				Q_re=A[index].re;
				Q_im=A[index].im;
				A[index].re=A[index1].re;
				A[index].im=A[index1].im;
				A[index1].re=Q_re;
				A[index1].im=Q_im;
			}
		}
//	obratnoe preobrazowanie
	if(NU == 0) goto m510;
	for(kk=1;kk<=N;kk++)
	{
		k=N1-kk;
		kc=k-1;
		if(B[kc] == 0) continue;
		index=kc*N+kc;
		tmp=(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
		Q_re=-A[index].re/tmp;
		Q_im=-A[index].im/tmp;
			for(j=0;j<NU;j++)
			{
				index=kc*M+j;
				tmp=Q_re*U[index].re - Q_im*U[index].im;
				tmp1=Q_im*U[index].re + Q_re*U[index].im;
				U[index].re=tmp;
				U[index].im=tmp1;
			}
			for(j=0;j<NU;j++)
			{
				Q_re=0; Q_im=0;
					for(i=kc;i<M;i++)
					{
						index=i*M+j;
						index1=i*N+kc;
							Q_re+=(U[index].re*A[index1].re + U[index].im*A[index1].im);
							Q_im+=(U[index].im*A[index1].re - U[index].re*A[index1].im);
					}
				index=kc*N+kc;
				tmp=B[kc]*(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
				Q_re/=tmp;
				Q_im/=tmp;
				for(i=kc;i<M;i++)
				{
					index=i*M+j;
					index1=i*N+kc;
					tmp=Q_re*A[index1].re - Q_im*A[index1].im;
					tmp1=Q_im*A[index1].re + Q_re*A[index1].im;
					U[index].re-=tmp;
					U[index].im-=tmp1;
				}
			}
	}
m510:
	if(NV == 0) goto m570;
	if(N < 1) goto m570;
		for(kk=2;kk<=N;kk++)
		{
			k=N1-kk;
			kc=k-1;
			k1=k+1;
			kc1=k1-1;
			if(C[kc1] == 0) continue;
			index=kc*N+kc1;
			tmp=(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
			Q_re=-A[index].re/tmp;
			Q_im=A[index].im/tmp;
				for(j=0;j<NV;j++)
				{
					index=kc1*N+j;
					tmp=Q_re*V[index].re - Q_im*V[index].im;
					tmp1=Q_im*V[index].re + Q_re*V[index].im;
				 	V[index].re=tmp;
				 	V[index].im=tmp1;
				}
				for(j=0;j<NV;j++)
				{
					Q_re=0; Q_im=0;
						for(i=kc1;i<N;i++)
						{
							index=i*N+j;
							index1=kc*N+i;
							Q_re+=(A[index1].re*V[index].re - A[index1].im*V[index].im);
							Q_im+=(A[index1].im*V[index].re + A[index1].re*V[index].im);
						}
					index=kc*N+kc1;
					tmp=C[kc1]*(float)sqrt(A[index].re*A[index].re + A[index].im*A[index].im);
					Q_re/=tmp;
					Q_im/=tmp;

					for(i=kc1;i<N;i++)
					{
						index=i*N+j;
						index1=kc*N+i;
						tmp=Q_re*A[index1].re + Q_im*A[index1].im;
						tmp1=Q_im*A[index1].re - Q_re*A[index1].im;
				 		V[index].re-=tmp;
				 		V[index].im-=tmp1;
					}

				}
		}
m570: IP=0;

delete [] B;
delete [] C;
delete [] T;

	return IP;
}
