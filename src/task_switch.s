.section .text

.global k_cli
.global k_sti

k_cli:
    cli
    ret

k_sti:
    sti
    ret
