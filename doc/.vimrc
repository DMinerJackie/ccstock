" ########Configure Neobundle########
" Note: Skip initialization for vim-tiny or vim-small.
if 0 | endif

if has('vim_startIng')
	if &compatible
		set nocompatible               " Be iMproved
	endif

	" Required:
	set runtimepath+=~/.vim/bundle/neobundle.vim/
	set runtimepath^=~/.vim/bundle/ctrlp.vim
endif

" Required:
call neobundle#begin(expand('~/.vim/bundle/'))

" Let NeoBundle manage NeoBundle
" Required:
NeoBundleFetch 'Shougo/neobundle.vim'

" My Bundles here:
" Refer to |:NeoBundle-examples|.
" Note: You don't set neobundle setting in .gvimrc!
NeoBundle 'tpope/vim-abolish'
NeoBundle 'altercation/vim-colors-solarized'
NeoBundle 'scrooloose/nerdtree'
NeoBundle 'Valloric/YouCompleteMe'
NeoBundle 'kien/ctrlp.vim'
NeoBundle 'othree/xml.vim'
NeoBundle 'vim-scripts/taglist.vim'
NeoBundle 'morhetz/gruvbox'

call neobundle#end()

" Required:
filetype plugin indent on

" If there are uninstalled bundles found on startup,
" this will conveniently prompt you to install them.
NeoBundleCheck


" ########Basic Setting########
syntax enable " syntax highlighting
set background=dark
" colorscheme solarized " Colorscheme from plugin
"colorscheme gruvbox
colorscheme desert

set number	" show line number
set autoindent	" copy indent from current line when staring a new line
set foldmethod=indent

set tabstop=4	" number of spaces that a <Tab> in the file counts for
set softtabstop=4	" number of spaces when inserting a <Tab>
set shiftwidth=4	" each step of (auto)indent
set expandtab	" convert tabs to spaces
set showcmd	" show current input command
set showmode	" show vim mode

set incsearch	" highlight while input
set hlsearch	" highlight searching result

set cursorline	" hightlight the screen line of the cursor
" hi CursorLine cterm=NONE ctermbg=DarkGrey ctermfg=white
" hi Folded ctermbg=LightCyan
hi CursorLine cterm=NONE ctermbg=LightCyan ctermfg=white
hi Folded ctermbg=DarkGrey
" ########Plugin Setting########
map <C-t> :NERDTreeToggle<CR>
let g:ycm_global_ycm_extra_conf = "~/.vim/.ycm_extra_conf.py"
let g:ycm_confirm_extra_conf = 0

" taglist
nnoremap <silent><F4> :TlistToggle<CR>
let Tlist_Show_One_File = 1            " 不同时显示多个文件的tag，只显示当前文件的
let Tlist_Exit_OnlyWindow = 1          " 如果taglist窗口是最后一个窗口，则退出vim
let Tlist_Use_Right_Window = 1         " 在右侧窗口中显示taglist窗口
let Tlist_File_Fold_Auto_Close=1       " 自动折叠当前非编辑文件的方法列表

" ########Custom Setting########
" Automatic create header
autocmd bufnewfile *.h\|*.cpp\|*.c\|*.cc so ~/.vim/custom/c_header.cfg
autocmd bufnewfile *.h\|*.cpp\|*.c\|*.cc exe "1," . 6 . "g/Creation Date:.*/s//Creation Date: " .strftime("%c")
autocmd Bufwritepre,filewritepre *.h\|*.cpp\|*.c\|*.cc execute "normal ma"
autocmd Bufwritepre,filewritepre *.h\|*.cpp\|*.c\|*.cc exe "1," . 6 . "g/Last Modified:.*/s/Last Modified:.*/Last Modified: " .strftime("%c")
autocmd bufwritepost,filewritepost *.h\|*.cpp\|*.c\|*.cc execute "normal `a"

" move between split window
nnoremap <C-h> <C-w>h
nnoremap <C-j> <C-w>j
nnoremap <C-k> <C-w>k
nnoremap <C-l> <C-w>l
