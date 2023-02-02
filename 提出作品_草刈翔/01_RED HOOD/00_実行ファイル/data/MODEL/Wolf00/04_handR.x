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
 168;
 -19.16885;-9.16386;-14.40219;,
 -20.31185;-5.20656;-13.92227;,
 -25.24944;-7.14447;-18.29935;,
 -18.65816;-3.94809;-17.30121;,
 -25.24944;-7.14447;-18.29935;,
 -16.79392;-6.52121;-19.67584;,
 -25.24944;-7.14447;-18.29935;,
 -16.58320;-9.98603;-18.22168;,
 -25.24944;-7.14447;-18.29935;,
 -19.16885;-9.16386;-14.40219;,
 -25.24944;-7.14447;-18.29935;,
 -21.92586;-1.21407;-8.14667;,
 -18.28878;2.75514;-9.80662;,
 -4.57761;-0.66040;-13.74121;,
 -4.36641;-6.12018;-13.11882;,
 -17.34832;-5.67025;-11.09969;,
 -34.05765;1.43358;-13.55642;,
 -32.31871;-0.90052;-11.06578;,
 -40.31348;-3.96642;-11.30323;,
 -33.35401;-0.87984;-7.37761;,
 -40.31348;-3.96642;-11.30323;,
 -36.12820;1.47489;-6.18019;,
 -40.31348;-3.96642;-11.30323;,
 -37.34923;3.79859;-10.51485;,
 -40.31348;-3.96642;-11.30323;,
 -37.34923;3.79859;-10.51485;,
 -34.05765;1.43358;-13.55642;,
 -40.31348;-3.96642;-11.30323;,
 -21.92586;-1.21407;-8.14667;,
 -18.28878;2.75514;-9.80662;,
 -21.61409;2.00133;-6.08781;,
 -22.96133;-1.19324;-4.45851;,
 -21.61409;2.00133;-6.08781;,
 -22.48845;3.40619;-1.28336;,
 -21.61409;2.00133;-6.08781;,
 -20.78450;7.86642;-5.87300;,
 -21.61409;2.00133;-6.08781;,
 -18.28878;2.75514;-9.80662;,
 -20.78450;7.86642;-5.87300;,
 -21.61409;2.00133;-6.08781;,
 -28.99781;3.57037;-12.14817;,
 -30.34493;0.37579;-10.51886;,
 -31.38013;0.39664;-6.83078;,
 -31.06829;3.61192;-4.77190;,
 -29.20345;6.79608;-8.24522;,
 -29.20345;6.79608;-8.24522;,
 -28.99781;3.57037;-12.14817;,
 -32.05865;1.44942;15.73136;,
 -29.07076;-0.89652;16.22480;,
 -34.85037;-3.96808;21.75054;,
 -27.16380;-0.89652;19.54717;,
 -34.85037;-3.96808;21.75054;,
 -28.24483;1.44942;22.37610;,
 -34.85037;-3.96808;21.75054;,
 -32.18629;3.79530;20.22149;,
 -34.85037;-3.96808;21.75054;,
 -32.18629;3.79530;20.22149;,
 -32.05865;1.44942;15.73136;,
 -34.85037;-3.96808;21.75054;,
 -27.50255;3.58331;13.11624;,
 -20.59593;2.69259;8.09216;,
 -19.70848;-1.21628;10.85117;,
 -27.29366;0.37889;15.20486;,
 -17.80145;-1.21628;14.17353;,
 -25.38665;0.37889;18.52725;,
 -16.10324;1.98844;15.40729;,
 -23.68845;3.58331;19.76096;,
 -17.26553;6.97611;11.65748;,
 -24.85091;6.78773;16.01121;,
 -24.85091;6.78773;16.01121;,
 -17.26553;6.97611;11.65748;,
 -20.59593;2.69259;8.09216;,
 -27.50255;3.58331;13.11624;,
 -36.19383;1.44942;1.75683;,
 -33.60731;-0.89652;3.33162;,
 -41.03352;-3.96808;6.29568;,
 -33.08077;-0.89652;7.12601;,
 -41.03352;-3.96808;6.29568;,
 -35.14100;1.44942;9.34557;,
 -41.03352;-3.96808;6.29568;,
 -37.99110;3.79530;5.87356;,
 -41.03352;-3.96808;6.29568;,
 -37.99110;3.79530;5.87356;,
 -36.19383;1.44942;1.75683;,
 -41.03352;-3.96808;6.29568;,
 -30.99044;3.58331;1.03486;,
 -22.48845;3.40619;-1.28336;,
 -22.91489;-1.21628;1.84817;,
 -31.57779;0.37889;3.05004;,
 -22.38845;-1.21628;5.64254;,
 -31.05150;0.37889;6.84444;,
 -20.59593;2.69259;8.09216;,
 -29.93763;3.58331;8.62363;,
 -20.95052;8.68121;3.50935;,
 -29.61361;6.78773;4.71124;,
 -29.61361;6.78773;4.71124;,
 -20.95052;8.68121;3.50935;,
 -22.48845;3.40619;-1.28336;,
 -30.99044;3.58331;1.03486;,
 -0.24441;0.00111;-7.97496;,
 2.78037;-0.01017;0.08030;,
 -0.24431;-5.10541;-6.95725;,
 -4.36641;-6.12018;-13.11882;,
 -4.57761;-0.66040;-13.74121;,
 -0.24431;-5.10541;-6.95725;,
 2.78037;-0.01017;0.08030;,
 -0.24441;-6.01984;0.08030;,
 -4.36641;-7.21435;0.08030;,
 -0.24441;-6.01984;0.08030;,
 2.78037;-0.01017;0.08030;,
 -0.24441;-5.10541;6.05485;,
 -4.36641;-4.81786;8.52850;,
 -0.24441;-5.10541;6.05485;,
 2.78037;-0.01017;0.08030;,
 -0.24441;0.00111;8.13554;,
 -4.36641;-0.01017;9.71864;,
 -0.24441;0.00111;8.13554;,
 2.78037;-0.01017;0.08030;,
 -0.24441;4.61917;5.77618;,
 -4.36641;5.51556;6.89563;,
 -0.24441;4.61917;5.77618;,
 2.78037;-0.01017;0.08030;,
 -0.24441;5.93748;0.08030;,
 -4.36641;7.09312;0.08030;,
 -0.24441;5.93748;0.08030;,
 2.78037;-0.01017;0.08030;,
 -0.24431;4.61917;-5.61561;,
 -4.36641;5.51556;-6.73504;,
 -0.24431;4.61917;-5.61561;,
 2.78037;-0.01017;0.08030;,
 -0.24441;0.00111;-7.97496;,
 -4.57761;-0.66040;-13.74121;,
 -18.28878;2.75514;-9.80662;,
 -21.92586;-1.21407;-8.14667;,
 -22.96133;-1.19324;-4.45851;,
 -10.31948;-4.25141;10.92913;,
 -17.80145;-1.21628;14.17353;,
 -19.70848;-1.21628;10.85117;,
 -22.38845;-1.21628;5.64254;,
 -4.36641;-7.21435;0.08030;,
 -22.91489;-1.21628;1.84817;,
 -22.96133;-1.19324;-4.45851;,
 -17.34832;-5.67025;-11.09969;,
 -17.34832;-5.67025;-11.09969;,
 -21.92586;-1.21407;-8.14667;,
 -20.78450;7.86642;-5.87300;,
 -20.95052;8.68121;3.50935;,
 -4.36641;7.09312;0.08030;,
 -17.26553;6.97611;11.65748;,
 -4.36641;7.09312;0.08030;,
 -20.95052;8.68121;3.50935;,
 -4.57761;-0.66040;-13.74121;,
 -18.28878;2.75514;-9.80662;,
 -20.78450;7.86642;-5.87300;,
 -17.26553;6.97611;11.65748;,
 -16.10324;1.98844;15.40729;,
 -22.91489;-1.21628;1.84817;,
 -22.48845;3.40619;-1.28336;,
 -20.95052;8.68121;3.50935;,
 -20.78450;7.86642;-5.87300;,
 -20.59593;2.69259;8.09216;,
 -17.26553;6.97611;11.65748;,
 -20.95052;8.68121;3.50935;,
 -10.31948;-4.25141;10.92913;,
 -17.80145;-1.21628;14.17353;,
 -10.31948;-4.25141;10.92913;,
 -4.36641;-7.21435;0.08030;,
 -4.36641;-7.21435;0.08030;;
 
 94;
 3;0,1,2;,
 3;1,3,4;,
 3;3,5,6;,
 3;5,7,8;,
 3;7,9,10;,
 4;3,1,11,12;,
 4;5,3,12,13;,
 4;7,5,13,14;,
 4;9,7,14,15;,
 3;16,17,18;,
 3;17,19,20;,
 3;19,21,22;,
 3;21,23,24;,
 3;25,26,27;,
 3;28,29,30;,
 3;31,28,32;,
 3;33,31,34;,
 3;35,33,36;,
 3;37,38,39;,
 4;40,29,28,41;,
 4;17,16,40,41;,
 4;41,28,31,42;,
 4;19,17,41,42;,
 4;42,31,33,43;,
 4;21,19,42,43;,
 4;43,33,35,44;,
 4;23,21,43,44;,
 4;45,38,37,46;,
 4;26,25,45,46;,
 3;47,48,49;,
 3;48,50,51;,
 3;50,52,53;,
 3;52,54,55;,
 3;56,57,58;,
 4;59,60,61,62;,
 4;48,47,59,62;,
 4;62,61,63,64;,
 4;50,48,62,64;,
 4;64,63,65,66;,
 4;52,50,64,66;,
 4;66,65,67,68;,
 4;54,52,66,68;,
 4;69,70,71,72;,
 4;57,56,69,72;,
 3;73,74,75;,
 3;74,76,77;,
 3;76,78,79;,
 3;78,80,81;,
 3;82,83,84;,
 4;85,86,87,88;,
 4;74,73,85,88;,
 4;88,87,89,90;,
 4;76,74,88,90;,
 4;90,89,91,92;,
 4;78,76,90,92;,
 4;92,91,93,94;,
 4;80,78,92,94;,
 4;95,96,97,98;,
 4;83,82,95,98;,
 3;99,100,101;,
 4;102,103,99,101;,
 3;104,105,106;,
 4;107,102,104,106;,
 3;108,109,110;,
 4;111,107,108,110;,
 3;112,113,114;,
 4;115,111,112,114;,
 3;116,117,118;,
 4;119,115,116,118;,
 3;120,121,122;,
 4;123,119,120,122;,
 3;124,125,126;,
 4;127,123,124,126;,
 3;128,129,130;,
 4;131,127,128,130;,
 3;132,133,134;,
 3;135,136,137;,
 4;135,137,138,139;,
 4;140,141,139,138;,
 3;139,141,142;,
 3;1,0,143;,
 4;142,141,144,1;,
 4;145,146,147,127;,
 4;148,119,149,150;,
 4;127,151,152,153;,
 4;115,119,154,155;,
 3;134,156,157;,
 3;158,159,157;,
 3;138,137,160;,
 3;161,162,160;,
 3;163,111,115;,
 4;164,163,115,155;,
 3;111,165,166;,
 3;142,102,167;;
 
 MeshMaterialList {
  2;
  94;
  0,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
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
  0;;
  Material {
   0.348000;0.373600;0.380000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  118;
  -0.580869;-0.261266;0.770928;,
  -0.427677;-0.520570;0.738986;,
  -0.692012;-0.461796;0.554854;,
  -0.409910;0.755001;-0.511808;,
  -0.081901;0.662042;-0.744978;,
  0.179737;-0.312113;-0.932888;,
  -0.372688;-0.861078;0.345901;,
  -0.795281;0.517596;-0.315631;,
  0.659603;-0.142310;-0.738019;,
  0.342976;-0.866214;-0.363375;,
  0.361677;-0.555511;-0.748731;,
  0.449970;-0.820815;-0.351837;,
  0.198759;-0.815784;0.543130;,
  -0.082391;-0.546619;0.833318;,
  -0.687757;0.548872;0.475111;,
  0.882918;-0.251584;0.396435;,
  0.309555;-0.502085;-0.807519;,
  0.135574;-0.912651;0.385600;,
  -0.161263;-0.499894;0.850941;,
  -0.341923;0.757772;0.555761;,
  -0.021865;-0.863948;-0.503106;,
  -0.282759;-0.551154;-0.785033;,
  0.061488;-0.818504;-0.571200;,
  0.524221;-0.818512;0.235014;,
  0.535219;-0.551167;0.640122;,
  -0.140922;0.545691;0.826052;,
  -0.368497;-0.518435;-0.771644;,
  0.367229;-0.914589;0.169320;,
  0.498291;-0.498242;0.709550;,
  0.239486;0.727139;0.643363;,
  -0.264254;-0.918377;-0.294540;,
  -0.073097;-0.991128;0.111003;,
  -0.196193;-0.960942;0.195191;,
  0.106817;0.957142;0.269201;,
  0.278480;-0.702809;0.654606;,
  0.382571;0.017204;0.923766;,
  0.308158;0.780260;0.544273;,
  0.234466;0.842699;-0.484648;,
  0.999798;0.020093;0.001068;,
  0.167829;-0.863945;-0.474798;,
  0.031259;-0.551156;-0.833817;,
  0.270603;-0.818500;-0.506786;,
  0.398358;-0.818501;0.413965;,
  0.257093;-0.551159;0.793805;,
  -0.439553;0.545685;0.713457;,
  -0.046955;-0.508485;-0.859789;,
  0.277321;-0.914581;0.294339;,
  0.188965;-0.508402;0.840131;,
  -0.077551;0.725064;0.684302;,
  0.176470;0.660906;-0.729425;,
  0.323363;-0.430250;0.842806;,
  0.339291;0.768191;0.542921;,
  0.198971;0.979987;0.005976;,
  0.884327;-0.141380;0.444946;,
  0.838936;0.147124;-0.523967;,
  0.844089;-0.438703;-0.308309;,
  0.631010;-0.775072;0.033023;,
  0.714810;-0.541758;0.442206;,
  0.730732;0.052357;0.680654;,
  0.690953;0.576636;0.435974;,
  0.637260;0.770649;0.000005;,
  0.714948;0.584091;-0.384301;,
  0.930650;-0.262428;0.254994;,
  0.950362;-0.205995;0.233191;,
  -0.520900;-0.530159;-0.669025;,
  -0.210756;-0.935068;0.285008;,
  -0.350605;-0.933476;-0.075486;,
  -0.230812;-0.963937;0.132485;,
  -0.314912;0.715452;-0.623666;,
  -0.685534;0.709997;-0.161082;,
  -0.354875;0.851993;-0.384932;,
  -0.119823;-0.390391;-0.912818;,
  -0.097698;-0.956593;0.274565;,
  -0.311509;-0.950222;-0.006339;,
  0.138553;-0.983138;0.119341;,
  -0.173466;-0.601080;0.780136;,
  -0.868777;0.290968;0.400705;,
  -0.338447;0.541876;-0.769301;,
  -0.532503;0.284228;-0.797280;,
  0.839689;-0.348244;0.416712;,
  0.743062;-0.280588;0.607560;,
  0.148151;-0.496400;-0.855359;,
  0.179205;-0.982226;0.055841;,
  -0.434310;-0.900276;0.029644;,
  -0.396117;-0.882228;0.254491;,
  -0.327519;-0.500295;0.801521;,
  -0.245161;0.757351;0.605240;,
  -0.079185;0.965038;0.249864;,
  0.027128;0.731218;-0.681604;,
  0.092997;0.699654;-0.708404;,
  0.433300;-0.605412;0.667628;,
  -0.321646;0.288029;0.901988;,
  -0.784354;0.545691;-0.294976;,
  -0.941077;0.288033;-0.177234;,
  0.162245;-0.982346;-0.093124;,
  0.352758;-0.493103;0.795243;,
  0.408203;0.604829;0.683778;,
  -0.441511;0.758473;-0.479361;,
  -0.328564;0.755270;-0.567110;,
  0.152260;-0.605410;0.781214;,
  -0.635558;0.288020;0.716317;,
  -0.617181;0.545679;-0.566853;,
  -0.806558;0.288016;-0.516247;,
  -0.220708;-0.512022;-0.830134;,
  0.185315;-0.982343;-0.025711;,
  0.013756;-0.511870;0.858953;,
  0.043704;0.691519;0.721035;,
  -0.282383;0.742957;-0.606856;,
  -0.199137;0.733148;-0.650261;,
  -0.922471;0.370010;-0.110181;,
  -0.995253;0.097020;0.007682;,
  -0.977928;0.208131;-0.018406;,
  -0.927064;0.312945;0.206440;,
  -0.947571;0.316468;-0.044250;,
  -0.882757;0.120174;0.454201;,
  -0.868592;0.205670;0.450830;,
  -0.847895;0.289619;0.444067;,
  0.319674;-0.295759;0.900186;;
  94;
  3;1,2,0;,
  3;69,3,70;,
  3;3,4,70;,
  3;5,5,71;,
  3;72,73,73;,
  4;3,69,7,68;,
  4;4,3,68,49;,
  4;5,5,8,8;,
  4;1,72,74,6;,
  3;10,11,9;,
  3;11,12,9;,
  3;12,13,75;,
  3;14,14,76;,
  3;77,77,78;,
  3;79,80,15;,
  3;62,79,15;,
  3;63,62,15;,
  3;53,63,15;,
  3;80,53,15;,
  4;16,81,81,16;,
  4;11,10,16,16;,
  4;82,83,84,17;,
  4;12,11,82,17;,
  4;85,84,85,18;,
  4;13,12,17,18;,
  4;19,86,87,19;,
  4;14,14,19,19;,
  4;88,89,68,88;,
  4;77,77,88,88;,
  3;21,22,20;,
  3;22,23,20;,
  3;23,24,90;,
  3;25,25,91;,
  3;92,92,93;,
  4;26,64,66,26;,
  4;22,21,26,26;,
  4;94,66,67,27;,
  4;23,22,94,27;,
  4;95,50,50,28;,
  4;24,23,27,28;,
  4;29,96,51,29;,
  4;25,25,29,29;,
  4;97,98,98,97;,
  4;92,92,97,97;,
  3;40,41,39;,
  3;41,42,39;,
  3;42,43,99;,
  3;44,44,100;,
  3;101,101,102;,
  4;45,103,30,45;,
  4;41,40,45,45;,
  4;104,30,65,46;,
  4;42,41,104,46;,
  4;105,65,105,47;,
  4;43,42,46,47;,
  4;48,106,33,48;,
  4;44,44,48,48;,
  4;107,108,108,107;,
  4;101,101,107,107;,
  3;54,38,55;,
  4;8,8,54,55;,
  3;55,38,56;,
  4;31,74,55,56;,
  3;56,38,57;,
  4;34,31,56,57;,
  3;57,38,58;,
  4;35,34,57,58;,
  3;58,38,59;,
  4;36,35,58,59;,
  3;59,38,60;,
  4;52,36,59,60;,
  3;60,38,61;,
  4;37,52,60,61;,
  3;61,38,54;,
  4;49,37,61,54;,
  3;68,7,109;,
  3;32,67,66;,
  4;32,66,65,31;,
  4;30,84,31,65;,
  3;31,84,6;,
  3;2,1,6;,
  4;6,84,83,2;,
  4;87,33,52,37;,
  4;51,36,52,33;,
  4;37,49,68,89;,
  4;35,36,51,96;,
  3;109,110,111;,
  3;112,113,111;,
  3;114,114,115;,
  3;116,112,115;,
  3;117,34,35;,
  4;50,117,35,50;,
  3;34,32,31;,
  3;6,74,31;;
 }
 MeshTextureCoords {
  168;
  0.166670;0.000000;,
  0.333330;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.416670;0.000000;,
  0.666670;0.000000;,
  0.583330;0.000000;,
  0.833330;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.916670;0.000000;,
  0.333330;0.486460;,
  0.500000;0.486460;,
  0.666670;0.486460;,
  0.833330;0.486460;,
  1.000000;0.486460;,
  0.000000;0.000000;,
  0.166670;0.000000;,
  0.083330;0.000000;,
  0.333330;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.416670;0.000000;,
  0.666670;0.000000;,
  0.583330;0.000000;,
  0.833330;0.000000;,
  1.000000;0.000000;,
  0.916670;0.000000;,
  0.166670;1.000000;,
  0.000000;1.000000;,
  0.083330;1.000000;,
  0.333330;1.000000;,
  0.250000;1.000000;,
  0.500000;1.000000;,
  0.416670;1.000000;,
  0.666670;1.000000;,
  0.583330;1.000000;,
  1.000000;1.000000;,
  0.833330;1.000000;,
  0.916670;1.000000;,
  0.000000;0.486460;,
  0.166670;0.486460;,
  0.333330;0.486460;,
  0.500000;0.486460;,
  0.666670;0.486460;,
  0.833330;0.486460;,
  1.000000;0.486460;,
  0.000000;0.000000;,
  0.166670;0.000000;,
  0.083330;0.000000;,
  0.333330;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.416670;0.000000;,
  0.666670;0.000000;,
  0.583330;0.000000;,
  0.833330;0.000000;,
  1.000000;0.000000;,
  0.916670;0.000000;,
  0.000000;0.486460;,
  0.000000;1.000000;,
  0.166670;1.000000;,
  0.166670;0.486460;,
  0.333330;1.000000;,
  0.333330;0.486460;,
  0.500000;1.000000;,
  0.500000;0.486460;,
  0.666670;1.000000;,
  0.666670;0.486460;,
  0.833330;0.486460;,
  0.833330;1.000000;,
  1.000000;1.000000;,
  1.000000;0.486460;,
  0.000000;0.000000;,
  0.166670;0.000000;,
  0.083330;0.000000;,
  0.333330;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.416670;0.000000;,
  0.666670;0.000000;,
  0.583330;0.000000;,
  0.833330;0.000000;,
  1.000000;0.000000;,
  0.916670;0.000000;,
  0.000000;0.486460;,
  0.000000;1.000000;,
  0.166670;1.000000;,
  0.166670;0.486460;,
  0.333330;1.000000;,
  0.333330;0.486460;,
  0.500000;1.000000;,
  0.500000;0.486460;,
  0.666670;1.000000;,
  0.666670;0.486460;,
  0.833330;0.486460;,
  0.833330;1.000000;,
  1.000000;1.000000;,
  1.000000;0.486460;,
  0.024160;1.000000;,
  0.062500;1.000000;,
  0.100840;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.149160;1.000000;,
  0.187500;1.000000;,
  0.225840;1.000000;,
  0.250000;1.000000;,
  0.274160;1.000000;,
  0.312500;1.000000;,
  0.350840;1.000000;,
  0.375000;1.000000;,
  0.399160;1.000000;,
  0.437500;1.000000;,
  0.475840;1.000000;,
  0.500000;1.000000;,
  0.524160;1.000000;,
  0.562500;1.000000;,
  0.600840;1.000000;,
  0.625000;1.000000;,
  0.649160;1.000000;,
  0.687500;1.000000;,
  0.725840;1.000000;,
  0.750000;1.000000;,
  0.774160;1.000000;,
  0.812500;1.000000;,
  0.850840;1.000000;,
  0.875000;1.000000;,
  0.899160;1.000000;,
  0.937500;1.000000;,
  0.975840;1.000000;,
  1.000000;1.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.968750;0.000000;,
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.475000;0.000000;,
  0.450000;0.000000;,
  0.250000;0.000000;,
  0.187500;0.000000;,
  0.031250;0.000000;,
  0.125000;0.613960;,
  0.074260;0.486460;,
  0.250000;0.486460;,
  0.437500;0.282460;,
  0.000000;0.000000;,
  0.750000;0.759270;,
  0.625000;0.564920;,
  0.750000;0.518550;,
  0.728480;0.000000;,
  1.000000;0.832700;,
  0.920070;0.661470;,
  0.875000;0.564920;,
  0.572260;0.607310;,
  0.500000;0.665400;,
  0.819300;0.000000;,
  0.816790;0.000000;,
  0.773260;0.000000;,
  0.833670;0.000000;,
  0.692940;0.000000;,
  0.665530;0.000000;,
  0.706970;0.000000;,
  0.375000;0.613960;,
  0.437500;0.639680;,
  0.375000;0.306980;,
  0.250000;0.380820;,
  0.250000;0.261640;;
 }
}
