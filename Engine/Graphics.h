/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Surface.h"
#include "Rect.h"
#include <cassert>

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	Color GetPixel( int x,int y ) const;
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	template<typename E>
	void DrawSprite( int x,int y,const Surface& s,E effect )
	{
		DrawSprite( x,y,s.GetRect(),s,effect );
	}
	template<typename E>
	void DrawSprite( int x,int y,const RectI& srcRect,const Surface& s,E effect )
	{
		DrawSprite( x,y,srcRect,GetScreenRect(),s,effect );
	}
	template<typename E>
	void DrawSprite( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,E effect )
	{
		assert( srcRect.left >= 0 );
		assert( srcRect.right <= s.GetWidth() );
		assert( srcRect.top >= 0 );
		assert( srcRect.bottom <= s.GetHeight() );
		if( x < clip.left )
		{
			srcRect.left += clip.left - x;
			x = clip.left;
		}
		if( y < clip.top )
		{
			srcRect.top += clip.top - y;
			y = clip.top;
		}
		if( x + srcRect.GetWidth() > clip.right )
		{
			srcRect.right -= x + srcRect.GetWidth() - clip.right;
		}
		if( y + srcRect.GetHeight() > clip.bottom )
		{
			srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
		}
		for( int sy = srcRect.top; sy < srcRect.bottom; sy++ )
		{
			for( int sx = srcRect.left; sx < srcRect.right; sx++ )
			{
				effect(
					s.GetPixel( sx,sy ),
					x + sx - srcRect.left,
					y + sy - srcRect.top,
					*this
				);
			}
		}
	}

	void DrawLine(const Vec2& pt1, const Vec2& pt2, Color c = Colors::White)
	{
		Vec2 pt = pt1;
		Vec2 dir = pt2 - pt1;
		const float len = dir.GetLength();
		dir = dir * (1.0f / len);
		for (size_t i = 0; i < size_t(len); i++)
		{
			PutPixel((int)pt.x, (int)pt.y, c);
			pt += dir;
		}
	}

	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
	static RectI GetScreenRect();
};

#include "SpriteEffect.h"

#ifndef GOD_GRAPHICS
extern template
void Graphics::DrawSprite<SpriteEffect::Copy>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::Copy effect );
extern template
void Graphics::DrawSprite<SpriteEffect::Chroma>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::Chroma effect );
extern template
void Graphics::DrawSprite<SpriteEffect::Substitution>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::Substitution effect );
#endif