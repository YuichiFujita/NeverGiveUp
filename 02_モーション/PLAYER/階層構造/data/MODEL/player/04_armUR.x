xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 156;
 9.88424;-3.09487;2.14003;,
 14.63119;-2.75247;2.39721;,
 14.48230;-1.66376;3.26332;,
 9.83122;-1.82105;3.88107;,
 9.81648;-3.86558;-1.28658;,
 15.05207;-3.58599;-0.32975;,
 14.87591;-3.63240;0.86799;,
 9.85257;-3.95524;0.05707;,
 16.16034;2.88277;0.97925;,
 15.95083;1.56072;2.83072;,
 17.06190;1.79382;2.75809;,
 17.36742;2.97679;0.81090;,
 14.38441;-0.33292;3.78597;,
 9.74123;-0.30548;4.39627;,
 15.12693;-2.59528;-1.90349;,
 17.43766;-2.66144;-1.42738;,
 17.19469;-3.53265;0.25899;,
 9.61982;3.32304;1.57131;,
 14.47981;2.73178;1.13017;,
 14.96051;3.14653;-0.59729;,
 9.62563;3.75102;-0.56022;,
 14.22904;1.15318;2.94555;,
 9.68793;1.50293;3.67275;,
 15.06666;2.32960;-1.95060;,
 17.91738;2.81277;-0.68041;,
 17.66740;2.02976;-1.88214;,
 9.70567;-0.32442;-3.82037;,
 15.27596;-0.49647;-2.80474;,
 9.73105;-2.73091;-2.78503;,
 9.65709;1.68931;-3.53396;,
 15.23277;1.18006;-2.68070;,
 9.62399;3.11783;-2.35464;,
 16.86772;-2.55923;2.72572;,
 17.00944;-1.24663;3.56624;,
 17.05823;-3.34331;1.39603;,
 17.38863;-0.01316;3.74635;,
 17.63363;-0.64046;-2.47430;,
 17.70811;0.82320;-2.49649;,
 18.38795;1.69267;2.74507;,
 18.59435;2.87165;0.74374;,
 16.70071;-1.72378;1.00991;,
 16.46547;-0.10426;0.62379;,
 16.57761;-1.23718;1.52599;,
 16.57761;-1.23718;1.52599;,
 16.46547;-0.10426;0.62379;,
 16.58415;-0.67547;2.09502;,
 16.58415;-0.67547;2.09502;,
 16.46547;-0.10426;0.62379;,
 16.62692;0.06828;2.29420;,
 16.62692;0.06828;2.29420;,
 16.46547;-0.10426;0.62379;,
 17.10551;1.04300;1.78824;,
 17.10551;1.04300;1.78824;,
 16.46547;-0.10426;0.62379;,
 17.20715;1.49296;0.63328;,
 17.20715;1.49296;0.63328;,
 16.46547;-0.10426;0.62379;,
 17.00961;1.39408;-0.06613;,
 17.00961;1.39408;-0.06613;,
 16.46547;-0.10426;0.62379;,
 17.00527;0.96274;-0.62917;,
 17.00527;0.96274;-0.62917;,
 16.46547;-0.10426;0.62379;,
 17.02564;0.35945;-0.93633;,
 17.02564;0.35945;-0.93633;,
 16.46547;-0.10426;0.62379;,
 16.98840;-0.37236;-0.92527;,
 16.98840;-0.37236;-0.92527;,
 16.46547;-0.10426;0.62379;,
 16.93765;-1.24453;-0.29310;,
 16.46547;-0.10426;0.62379;,
 16.70071;-1.72378;1.00991;,
 16.76892;-1.81846;0.44141;,
 16.93765;-1.24453;-0.29310;,
 16.46547;-0.10426;0.62379;,
 16.76892;-1.81846;0.44141;,
 16.86772;-2.55923;2.72572;,
 17.05823;-3.34331;1.39603;,
 17.00944;-1.24663;3.56624;,
 17.38863;-0.01316;3.74635;,
 18.38795;1.69267;2.74507;,
 18.59435;2.87165;0.74374;,
 17.91738;2.81277;-0.68041;,
 17.66740;2.02976;-1.88214;,
 17.70811;0.82320;-2.49649;,
 17.63363;-0.64046;-2.47430;,
 17.70811;0.82320;-2.49649;,
 17.43766;-2.66144;-1.42738;,
 17.19469;-3.53265;0.25899;,
 5.61421;3.68785;-2.85902;,
 5.59986;4.41987;-0.56316;,
 8.56513;4.80147;-0.67737;,
 8.56065;4.02483;-2.97400;,
 3.14597;4.33359;-0.13753;,
 3.14342;3.60948;-2.54143;,
 -1.23739;2.98606;-1.93107;,
 -1.48942;3.12232;0.46612;,
 -1.08034;2.74407;2.67350;,
 2.89611;3.58235;2.42801;,
 -3.10804;0.87938;0.27939;,
 -3.15252;0.40432;2.36949;,
 -3.05911;0.70325;-1.40586;,
 5.32976;-0.38854;-4.84163;,
 2.94487;-0.38188;-5.10616;,
 3.03376;1.81569;-4.41708;,
 5.60280;1.92147;-4.41390;,
 4.68356;-4.68773;-2.34613;,
 4.91281;-3.22606;-4.05312;,
 8.84828;-3.27712;-3.65580;,
 8.89948;-4.75625;-1.84654;,
 0.18151;-0.22925;-5.11589;,
 -1.82868;-1.81387;-4.19833;,
 -2.51135;-0.62307;-3.25197;,
 -0.54309;1.61448;-3.91350;,
 1.72781;-3.11034;-4.43784;,
 5.27464;3.82441;2.06016;,
 8.75634;2.23590;-4.50234;,
 8.79735;-0.28973;-4.90113;,
 8.79865;-0.27927;5.55228;,
 8.73593;1.98411;4.65772;,
 9.05201;-4.82716;0.13796;,
 9.07872;-3.94803;2.59923;,
 8.69599;4.26013;2.02229;,
 8.99255;-2.11925;4.79103;,
 4.99047;-4.25950;2.63146;,
 5.00216;-2.50330;4.53935;,
 4.71565;-5.07328;-0.18751;,
 4.96328;-0.59749;5.62568;,
 0.78869;-4.13728;-2.68213;,
 -2.21872;-3.16134;-2.15489;,
 0.70707;-3.97635;2.32864;,
 1.39860;-2.51632;4.76891;,
 -2.09314;-2.11148;3.05605;,
 -2.34468;-2.89632;1.50170;,
 0.39196;-4.61020;-0.02588;,
 -2.53641;-3.41257;-0.06522;,
 0.18151;-0.22925;-5.11589;,
 2.94487;-0.38188;-5.10616;,
 1.72781;-3.11034;-4.43784;,
 5.04721;1.60234;4.53007;,
 -0.75351;1.31446;4.80962;,
 -1.97344;-0.54747;4.02635;,
 0.46637;-0.65016;5.59586;,
 2.21582;-0.66693;5.70366;,
 2.75902;1.52890;4.72954;,
 -2.97584;-0.89845;2.81602;,
 -3.09131;-1.83120;-2.75563;,
 -3.53350;-1.54952;-0.73554;,
 -3.46220;-1.20330;0.89053;,
 -1.97344;-0.54747;4.02635;,
 -2.09314;-2.11148;3.05605;,
 0.46637;-0.65016;5.59586;,
 1.39860;-2.51632;4.76891;,
 0.46637;-0.65016;5.59586;,
 -2.09314;-2.11148;3.05605;,
 -1.85031;0.79383;3.89363;;
 
 126;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;3,2,12,13;,
 4;5,14,15,16;,
 4;17,18,19,20;,
 4;7,6,1,0;,
 4;13,12,21,22;,
 4;23,19,24,25;,
 4;26,27,14,28;,
 4;29,30,27,26;,
 4;31,23,30,29;,
 4;1,32,33,2;,
 4;5,16,34,6;,
 4;2,33,35,12;,
 4;18,8,24,19;,
 4;6,34,32,1;,
 4;12,35,9,21;,
 4;27,36,15,14;,
 4;30,37,36,27;,
 4;23,25,37,30;,
 3;35,38,10;,
 3;11,24,8;,
 4;18,21,9,8;,
 3;24,11,39;,
 3;10,9,35;,
 4;11,10,38,39;,
 4;17,22,21,18;,
 4;4,28,14,5;,
 4;31,20,19,23;,
 3;40,41,42;,
 3;43,44,45;,
 3;46,47,48;,
 3;49,50,51;,
 3;52,53,54;,
 3;55,56,57;,
 3;58,59,60;,
 3;61,62,63;,
 3;64,65,66;,
 3;67,68,69;,
 3;70,71,72;,
 3;73,74,75;,
 4;40,42,76,77;,
 4;43,45,78,76;,
 4;79,78,46,48;,
 4;80,79,49,51;,
 4;52,54,81,80;,
 4;55,57,82,81;,
 4;83,82,58,60;,
 4;61,63,84,83;,
 4;64,66,85,86;,
 4;67,69,87,85;,
 4;73,75,88,87;,
 4;88,72,71,77;,
 4;89,90,91,92;,
 4;93,94,95,96;,
 4;97,98,93,96;,
 4;99,100,97,96;,
 4;95,101,99,96;,
 4;102,103,104,105;,
 4;104,94,89,105;,
 4;106,107,108,109;,
 4;110,111,112,113;,
 4;104,103,110,113;,
 4;95,94,104,113;,
 4;112,101,95,113;,
 4;114,103,102,107;,
 4;93,98,115,90;,
 4;89,94,93,90;,
 4;92,31,29,116;,
 4;116,29,26,117;,
 4;117,26,28,108;,
 4;118,13,22,119;,
 4;120,7,0,121;,
 4;122,17,20,91;,
 4;123,3,13,118;,
 4;109,4,7,120;,
 4;121,0,3,123;,
 4;124,121,123,125;,
 4;106,109,120,126;,
 4;125,123,118,127;,
 4;115,122,91,90;,
 4;126,120,121,124;,
 4;102,117,108,107;,
 4;105,116,117,102;,
 4;89,92,116,105;,
 4;122,119,22,17;,
 4;109,108,28,4;,
 4;92,91,20,31;,
 3;111,110,114;,
 4;114,107,106,128;,
 4;129,111,114,128;,
 4;130,124,125,131;,
 4;132,133,130,131;,
 4;128,106,126,134;,
 4;130,134,126,124;,
 4;133,135,134,130;,
 4;134,135,129,128;,
 3;136,137,138;,
 3;110,103,114;,
 4;139,119,122,115;,
 4;119,139,127,118;,
 3;140,141,142;,
 4;141,132,131,142;,
 3;142,131,143;,
 4;139,144,143,127;,
 4;140,142,143,144;,
 4;115,98,144,139;,
 4;98,97,140,144;,
 4;143,131,125,127;,
 3;132,141,145;,
 3;141,100,145;,
 3;112,111,146;,
 3;111,129,146;,
 4;147,101,112,146;,
 4;147,146,129,135;,
 4;147,135,133,148;,
 4;147,148,99,101;,
 4;148,145,100,99;,
 4;148,133,132,145;,
 3;149,150,151;,
 3;152,153,154;,
 3;141,140,155;,
 3;140,97,155;,
 3;97,100,155;,
 3;100,141,155;;
 
 MeshMaterialList {
  2;
  126;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.674400;0.511200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.382400;0.708800;0.768800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  124;
  0.017618;-0.025318;0.999524;,
  -0.539493;0.841255;0.035184;,
  -0.038995;0.454147;-0.890073;,
  -0.030542;-0.996713;0.075030;,
  0.023485;-0.629675;0.776503;,
  -0.402932;0.501239;-0.765771;,
  0.051431;-0.572349;-0.818395;,
  -0.090224;0.995127;-0.039768;,
  -0.761454;0.260073;-0.593760;,
  -0.924824;0.376718;0.052773;,
  -0.095899;0.837392;-0.538124;,
  -0.519491;0.743958;0.420304;,
  0.043172;-0.053024;-0.997660;,
  -0.009791;-0.918909;-0.394347;,
  -0.517732;0.754121;-0.404048;,
  -0.053743;0.886812;0.458995;,
  -0.180109;-0.021644;-0.983408;,
  -0.002626;-0.881666;0.471867;,
  -0.065531;0.538643;-0.839982;,
  -0.135188;0.990820;-0.000731;,
  -0.365155;-0.272604;0.890140;,
  -0.656522;-0.136116;0.741924;,
  -0.546611;-0.296737;-0.783047;,
  -0.849490;0.309844;0.427041;,
  -0.891550;0.369804;-0.261502;,
  -0.138321;0.867898;-0.477096;,
  -0.070415;0.889192;0.452082;,
  0.014906;-0.022348;-0.999639;,
  0.044571;0.628075;0.776875;,
  0.139272;-0.655235;-0.742476;,
  0.034910;-0.486847;0.872789;,
  0.099852;0.983094;-0.153478;,
  0.036633;-0.967627;0.249710;,
  0.143079;0.317048;-0.937555;,
  0.096980;-0.958669;-0.267486;,
  0.128641;0.729376;-0.671909;,
  0.057644;0.903394;0.424918;,
  0.008122;-0.780797;0.624732;,
  0.148014;-0.178118;-0.972813;,
  0.058488;0.087076;0.994483;,
  0.006522;0.684663;0.728830;,
  0.130127;-0.681361;-0.720287;,
  -0.023784;-0.429908;0.902559;,
  0.026703;0.987039;-0.158241;,
  0.030011;-0.955172;0.294528;,
  0.113083;0.271166;-0.955867;,
  -0.019975;-0.740234;0.672053;,
  0.126583;-0.208949;-0.969699;,
  -0.014535;0.311509;0.950132;,
  0.091334;-0.972195;-0.215627;,
  0.104893;0.688613;-0.717502;,
  0.001448;0.974704;0.223494;,
  -0.057271;0.950250;0.306177;,
  0.087385;0.968977;0.231186;,
  0.067789;0.690747;0.719912;,
  -0.059815;0.662034;0.747083;,
  0.462217;-0.831277;-0.308762;,
  0.471093;0.710026;-0.523388;,
  0.468474;0.792540;0.390400;,
  0.482376;0.530485;0.697065;,
  0.486365;-0.537992;-0.688487;,
  0.462247;-0.483089;0.743608;,
  0.458799;0.884687;-0.082657;,
  0.432394;-0.888565;0.153257;,
  0.502181;0.346889;-0.792138;,
  0.426342;-0.757618;0.494214;,
  0.500829;-0.115698;-0.857778;,
  0.503210;0.053250;0.862522;,
  0.438551;-0.827495;-0.350606;,
  0.180679;0.853307;-0.489104;,
  0.354267;0.841968;0.406921;,
  0.382356;0.551556;0.741343;,
  0.444278;-0.516981;-0.731675;,
  0.434660;-0.525611;0.731302;,
  0.335927;0.939800;-0.062687;,
  0.430485;-0.897317;0.097491;,
  0.396522;0.400099;-0.826251;,
  0.434904;-0.781745;0.446915;,
  0.428859;-0.085516;-0.899315;,
  0.438221;0.016349;0.898719;,
  -0.209502;-0.892349;-0.399777;,
  -0.224922;-0.610766;0.759194;,
  -0.558011;-0.527661;0.640467;,
  -0.550480;-0.744610;-0.377528;,
  -0.075840;-0.461978;-0.883643;,
  -0.222759;-0.871506;0.436871;,
  -0.644183;-0.677821;0.354383;,
  -0.242759;-0.968469;0.055996;,
  -0.655801;-0.753520;0.046181;,
  -0.017480;0.594032;0.804251;,
  -0.440969;0.417613;0.794447;,
  0.000248;-0.026486;0.999649;,
  -0.008497;0.603399;0.797395;,
  -0.981884;-0.170622;-0.082410;,
  -0.968888;-0.179903;0.169970;,
  -0.867889;-0.176456;0.464362;,
  -0.854534;-0.245654;-0.457631;,
  -0.724077;0.358253;0.589378;,
  0.990671;-0.093973;0.098688;,
  0.978422;0.195807;0.065958;,
  0.990628;0.131017;-0.038607;,
  0.983616;0.002771;-0.180254;,
  0.918859;-0.310409;-0.243608;,
  0.803513;-0.556639;-0.210995;,
  0.813488;-0.546884;-0.197877;,
  0.899249;-0.437305;-0.010761;,
  0.936284;-0.234817;0.261215;,
  0.934320;-0.089560;0.344999;,
  0.942115;0.047379;0.331925;,
  0.944062;0.139819;0.298658;,
  0.966487;0.171942;0.190629;,
  0.984011;0.155075;-0.087602;,
  0.972567;0.228075;0.045779;,
  0.959310;-0.003429;-0.282334;,
  0.836522;-0.359492;-0.413518;,
  0.673410;-0.650297;-0.351615;,
  0.705805;-0.662224;-0.251590;,
  0.848680;-0.528543;-0.019601;,
  0.920142;-0.279057;0.274712;,
  0.922111;-0.096222;0.374769;,
  0.934800;0.037820;0.353155;,
  0.946788;0.135307;0.292034;,
  0.965173;0.194460;0.175006;,
  0.742810;0.458129;-0.488213;;
  126;
  4;65,37,30,61;,
  4;56,34,32,63;,
  4;52,55,40,51;,
  4;61,30,39,67;,
  4;34,29,41,49;,
  4;58,36,31,62;,
  4;63,32,37,65;,
  4;67,39,28,59;,
  4;35,31,43,50;,
  4;66,38,29,60;,
  4;64,33,38,66;,
  4;57,35,33,64;,
  4;37,46,42,30;,
  4;34,49,44,32;,
  4;30,42,48,39;,
  4;36,52,43,31;,
  4;32,44,46,37;,
  4;39,48,55,28;,
  4;38,47,41,29;,
  4;33,45,47,38;,
  4;35,50,45,33;,
  3;48,54,40;,
  3;51,43,52;,
  4;36,28,55,52;,
  3;43,51,53;,
  3;40,55,48;,
  4;51,40,54,53;,
  4;58,59,28,36;,
  4;56,60,29,34;,
  4;57,62,31,35;,
  3;99,98,100;,
  3;100,98,101;,
  3;101,98,102;,
  3;102,98,103;,
  3;103,98,104;,
  3;104,98,105;,
  3;105,98,106;,
  3;106,98,107;,
  3;107,98,108;,
  3;108,98,109;,
  3;98,99,110;,
  3;109,98,110;,
  4;99,100,111,112;,
  4;100,101,113,111;,
  4;114,113,101,102;,
  4;115,114,102,103;,
  4;103,104,116,115;,
  4;104,105,117,116;,
  4;118,117,105,106;,
  4;106,107,119,118;,
  4;107,108,120,119;,
  4;108,109,121,120;,
  4;109,110,122,121;,
  4;122,110,99,112;,
  4;10,7,74,69;,
  4;19,25,14,1;,
  4;11,26,19,1;,
  4;9,23,11,1;,
  4;14,24,9,1;,
  4;12,27,18,2;,
  4;18,25,10,2;,
  4;13,6,72,68;,
  4;16,22,8,5;,
  4;18,27,16,5;,
  4;14,25,18,5;,
  4;8,24,14,5;,
  4;84,27,12,6;,
  4;19,26,15,7;,
  4;10,25,19,7;,
  4;123,57,64,76;,
  4;76,64,66,78;,
  4;78,66,60,72;,
  4;79,67,59,71;,
  4;75,63,65,77;,
  4;70,58,62,74;,
  4;73,61,67,79;,
  4;68,56,63,75;,
  4;77,65,61,73;,
  4;17,77,73,4;,
  4;13,68,75,3;,
  4;4,73,79,0;,
  4;15,70,74,7;,
  4;3,75,77,17;,
  4;12,78,72,6;,
  4;2,76,78,12;,
  4;10,69,76,2;,
  4;70,71,59,58;,
  4;68,72,60,56;,
  4;69,74,62,57;,
  3;22,16,84;,
  4;84,6,13,80;,
  4;83,22,84,80;,
  4;85,17,4,81;,
  4;82,86,85,81;,
  4;80,13,3,87;,
  4;85,87,3,17;,
  4;86,88,87,85;,
  4;87,88,83,80;,
  3;16,27,84;,
  3;16,27,84;,
  4;89,71,70,15;,
  4;71,89,0,79;,
  3;90,21,20;,
  4;21,82,81,20;,
  3;20,81,91;,
  4;89,92,91,0;,
  4;90,20,91,92;,
  4;15,26,92,89;,
  4;26,11,90,92;,
  4;91,81,4,0;,
  3;82,21,95;,
  3;21,23,95;,
  3;8,22,96;,
  3;22,83,96;,
  4;93,24,8,96;,
  4;93,96,83,88;,
  4;93,88,86,94;,
  4;93,94,9,24;,
  4;94,95,23,9;,
  4;94,86,82,95;,
  3;21,82,20;,
  3;81,20,82;,
  3;21,90,97;,
  3;90,11,97;,
  3;11,23,97;,
  3;23,21,97;;
 }
 MeshTextureCoords {
  156;
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.354170;0.750000;,
  0.375000;1.000000;,
  0.395830;0.750000;,
  0.437500;0.750000;,
  0.458330;1.000000;,
  0.479170;0.750000;,
  0.520830;0.750000;,
  0.541670;1.000000;,
  0.562500;0.750000;,
  0.604170;0.750000;,
  0.625000;1.000000;,
  0.645830;0.750000;,
  0.687500;0.750000;,
  0.708330;1.000000;,
  0.729170;0.750000;,
  0.770830;0.750000;,
  0.791670;1.000000;,
  0.812500;0.750000;,
  0.854170;0.750000;,
  0.875000;1.000000;,
  0.895830;0.750000;,
  0.937500;0.750000;,
  0.958330;1.000000;,
  0.979170;0.750000;,
  0.020830;0.750000;,
  0.041670;1.000000;,
  0.062500;0.750000;,
  0.104170;0.750000;,
  0.125000;1.000000;,
  0.145830;0.750000;,
  0.291670;1.000000;,
  0.312500;0.750000;,
  0.270830;0.750000;,
  0.187500;0.750000;,
  0.208330;1.000000;,
  0.229170;0.750000;,
  0.416670;0.500000;,
  0.333330;0.500000;,
  0.500000;0.500000;,
  0.583330;0.500000;,
  0.666670;0.500000;,
  0.750000;0.500000;,
  0.833330;0.500000;,
  0.916670;0.500000;,
  1.000000;0.500000;,
  0.083330;0.500000;,
  0.000000;0.500000;,
  0.166670;0.500000;,
  0.250000;0.500000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;;
 }
}
