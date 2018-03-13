set number
syntax on
set autoindent
set cindent
set smartindent
set tabstop=4
set shiftwidth=4
set softtabstop=4
set hlsearch
set expandtab
autocmd FileType make setlocal noexpandtab
nnoremap <F2> :set invpaste paste?<CR>
set pastetoggle=<F2>


func! Man()
    let sm = expand("<cword>")
    exe "!man -S 2:3:4:5:6:7:8:9:tcl:n:l:p:o ".sm
endfunc
nmap ,ma :call Man()<CR><CR>

