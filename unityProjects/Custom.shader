Shader "Custom/Custom"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_SubTex ("SubTexture", 2D) = "white" {}
        _Red("赤色の乗算値", float) = 1.0
        _Green("緑色の乗算値", float) = 1.0
        _Blue ("青色の乗算値", float) = 1.0
        _Brightness ("明るさの加算値", Range(0.0, 2.0)) = 1.0
	}
	SubShader
	{
		Tags { "RenderType"="Opaque" }
		LOD 100

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			// make fog work
			#pragma multi_compile_fog
			
			#include "UnityCG.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				UNITY_FOG_COORDS(1)
				float4 vertex : SV_POSITION;
			};

            sampler2D _MainTex;
            sampler2D _SubTex;
			float4 _MainTex_ST;
            float _Red;
            float _Green;
            float _Blue;
            float _Brightness;
			
			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = mul(UNITY_MATRIX_MVP, v.vertex);
				o.uv = TRANSFORM_TEX(v.uv, _MainTex);
				UNITY_TRANSFER_FOG(o,o.vertex);
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				// sample the texture
				fixed4 col = tex2D(_MainTex, i.uv);
				//fixed4 subTexCol = tex2D(_SubTex, i.uv);

                //col += subTexCol;

                col.r *= _Red;
                col.g *= _Green;
                col.b *= _Blue;

                float brightness = -1.0 + _Brightness;
                col += brightness;

				return col;
			}
			ENDCG
		}
	}
}
