local Objeto = {}

-- Função construtora para criar uma nova loja
function Objeto.create(opts)
    opts = opts or {}

    return {
        rect = opts.rect or {},
        hover = false,
        img = opts.img,
        img_hover = opts.img_hover or opts.img,

        get_img = function(self)
            return self.hover and self.img_hover or self.img
        end
    }
end

return Objeto