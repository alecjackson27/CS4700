import Control.Parallel (par, pseq)

forceEval :: [a] -> ()
forceEval xs = go xs `pseq` ()
    where go (_:xs) = go xs
          go [] = 1

parSort :: (Ord a) => [a] -> [a]
parSort (x:xs)    = forceEval greater `par` (forceEval lesser `pseq`
                                         (lesser ++ x:greater))
    where lesser  = parSort [y | y <- xs, y <  x]
          greater = parSort [y | y <- xs, y >= x]
parSort _         = []