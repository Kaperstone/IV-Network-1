/*
* Copyright (C) GTA-Network Team
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
*     * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following disclaimer
* in the documentation and/or other materials provided with the
* distribution.
*     * Neither the name of GTA-Network nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "CIVModelInfo.h"
#include <CCore.h>
#include "CIVModelManager.h"

extern CCore * g_pCore;

CIVModelInfo::CIVModelInfo() :
	m_iModelIndex(-1)
{
	// Reset the model index
	m_iModelIndex = -1;
}

CIVModelInfo::CIVModelInfo(int iModelIndex) :
	m_iModelIndex(iModelIndex)
{
}

CIVModelInfo::~CIVModelInfo( )
{

}

IVBaseModelInfo * CIVModelInfo::GetModelInfo( )
{
	if( m_iModelIndex >= 0 && m_iModelIndex < NUM_ModelInfos )
		return *(IVBaseModelInfo **)((g_pCore->GetBase() + ARRAY_ModelInfos) + (m_iModelIndex * 4));

	return nullptr;
}

BYTE CIVModelInfo::GetType( )
{
	IVBaseModelInfo * pModelInfo = GetModelInfo( );
	if( pModelInfo )
	{
		CLogFile::Printf("Am I Crashed? (%s %i)", __FILE__, __LINE__);
		
		return pModelInfo->GetType();
	}

	return 0;
}

void CIVModelInfo::Load( bool bWaitForLoad )
{
	// Is the model invalid?
	if( !IsValid() )
		return;

	// Are we already loaded
	if( IsLoaded() )
		return;

	// Request the model
	CIVModelManager::RequestModel( m_iModelIndex );

	// Should we wait for it to load?
	if( bWaitForLoad )
	{
		// Load all requested models
		CIVModelManager::LoadRequestedModels( );

		// Loop until the model loads
		while( !IsLoaded() )
			Sleep(10);
	}
}

bool CIVModelInfo::IsLoaded( )
{
	if( IsValid() )
		return CIVModelManager::HasModelLoaded( m_iModelIndex );

	return false;
}

DWORD CIVModelInfo::GetHash( )
{
	IVBaseModelInfo * pModelInfo = GetModelInfo( );

	if( pModelInfo )
		return pModelInfo->dwHash;

	return NULL;
}

DWORD CIVModelInfo::GetReferenceCount( )
{
	IVBaseModelInfo * pModelInfo = GetModelInfo( );

	if( pModelInfo )
		return pModelInfo->dwRefCount;

	return NULL;
}

WORD CIVModelInfo::GetAnimIndex( )
{
	IVBaseModelInfo * pModelInfo = GetModelInfo( );

	if( pModelInfo )
		return pModelInfo->wAnimIndex;

	return NULL;
}

void CIVModelInfo::AddReference( bool bWaitForLoad )
{
	// Increase the ref count
	m_dwReferenceCount++;

	// Is this the first reference?
	if( m_dwReferenceCount == 1 || !IsLoaded())
	{
		// Load the model
		Load( bWaitForLoad );
	}
}

void CIVModelInfo::RemoveReference( )
{
	// Is there a reference to this model?
	if( m_dwReferenceCount > 0 )
	{
		// Decrease the reference count
		m_dwReferenceCount--;
	}
}