//////////////////////////////////////////////////////////////////////////////
//
// License Agreement:
//
// The following are Copyright � 2007, mC2 Team
//
// Sources and Binaries of: mC2, win32cpp
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright 
//      notice, this list of conditions and the following disclaimer in the 
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may 
//      be used to endorse or promote products derived from this software 
//      without specific prior written permission. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE. 
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.hpp"
#include <server/SyncpathController.hpp>
#include <server/SyncpathView.hpp>
#include <server/SyncpathListController.hpp>
#include <core/Indexer.h>

#include <win32cpp/FolderBrowseDialog.hpp>
#include <win32cpp/Button.hpp>
#include <win32cpp/Label.hpp>
#include <win32cpp/Window.hpp>
#include <win32cpp/Application.hpp>

//////////////////////////////////////////////////////////////////////////////

using namespace win32cpp;
using namespace musik::server;

//////////////////////////////////////////////////////////////////////////////

SyncpathController::SyncpathController(SyncpathView& syncpathView,musik::core::Indexer *indexer)
: syncpathView(syncpathView)
, indexer(indexer)
{
    this->syncpathView.Handle()
        ? this->OnViewCreated(&this->syncpathView)
        : this->syncpathView.Created.connect(this, &SyncpathController::OnViewCreated);
    
    this->syncpathView.Resized.connect(
        this, &SyncpathController::OnViewResized);
}

void SyncpathController::OnViewCreated(Window* window)
{

    this->syncpathView.addPathButton->Pressed.connect(this,&SyncpathController::OnAddPath);
    this->syncpathView.removePathButton->Pressed.connect(this,&SyncpathController::OnRemovePath);

    this->syncpathListController.reset(new musik::server::SyncpathListController(*this->syncpathView.pathList,this));

}

void        SyncpathController::OnViewResized(Window* window, Size size)
{
}

void SyncpathController::OnAddPath(Button* button){
    win32cpp::FolderBrowseDialog addPath;

    if(addPath.Show(win32cpp::Application::Instance().MainWindow())==win32cpp::FolderBrowseDialog::ResultOK){
        if(this->indexer){
            this->indexer->AddPath(addPath.Directory());
        }
    }
}

void SyncpathController::OnRemovePath(Button* button){
    this->syncpathListController->RemoveSelectedPaths();
}


