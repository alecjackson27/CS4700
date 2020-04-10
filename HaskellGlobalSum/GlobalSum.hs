module GlobalSum where

import Control.Parallel (par, pseq)

sum' [] = 0
sum' (x:xs) = x + sum' xs

parGlobalSum arr
    | length arr == 0 = 0
    | length arr == 1 = arr !! 0
    | otherwise = upper `par` (lower `pseq` (lower + upper))
        where 
            lower = parGlobalSum [arr !! i | i <- [0..len `div` 2]]
            upper = parGlobalSum [arr !! i | i <- [len `div` 2 + 1..len]]
            len = length arr - 1

globalSum arr
    | length arr == 0 = 0
    | length arr == 1 = arr !! 0
    | otherwise = lower + upper
        where 
            lower = globalSum [arr !! i | i <- [0..len `div` 2]]
            upper = globalSum [arr !! i | i <- [len `div` 2 + 1..len]]
            len = length arr - 1

parGlobalSumTest arr
    | length arr == 0 = 0
    | length arr == 1 = arr !! 0
    | otherwise = upper `par` (lower `pseq` (lower + upper))
        where
            lower = parGlobalSumTest (fst split)
            upper = parGlobalSumTest (snd split)
            split = splitAt (length arr `div` 2) arr