module AlgebraicGraph where

import qualified Data.Set as S
import Distribution.Simple.Utils (xargs)

data AlgebraicGraph a
    = Empty
    | Node a
    | Overlay (AlgebraicGraph a) (AlgebraicGraph a)
    | Connect (AlgebraicGraph a) (AlgebraicGraph a)
    deriving (Eq, Show)

-- Constructor check
isEmpty :: Ord a => AlgebraicGraph a -> Bool
isEmpty Empty = True
isEmpty _     = False

isNode :: Ord a => AlgebraicGraph a -> Bool
isNode (Node _) = True
isNode _     = False

isOverlay :: Ord a => AlgebraicGraph a -> Bool
isOverlay (Overlay _ _) = True
isOverlay _     = False

isConnect :: Ord a => AlgebraicGraph a -> Bool
isConnect (Connect _ _) = True
isConnect _     = False

-- (1, 2), (1, 3)
angle :: AlgebraicGraph Int
angle = Connect (Node 1) (Overlay (Node 2) (Node 3))

-- (1, 2), (1, 3), (2, 3)
triangle :: AlgebraicGraph Int
triangle = Connect (Node 1) (Connect (Node 2) (Node 3))

{-
    *** TODO ***

    Mulțimea nodurilor grafului.

    Hint: S.union
-}
nodes :: Ord a => AlgebraicGraph a -> S.Set a
nodes Empty = S.empty
nodes (Node x) = S.fromList [x]
nodes (Overlay x y) = S.union (nodes x) (nodes y)
nodes (Connect x y) = S.union (nodes x) (nodes y)

{-
    *** TODO ***

    Mulțimea arcelor grafului.

    Hint: S.union, S.cartesianProduct
-}
edges :: Ord a => AlgebraicGraph a -> S.Set (a, a)
edges (Overlay x y) = S.union (edges x) (edges y)
edges (Connect x y) = S.union (S.cartesianProduct (nodes x) (nodes y)) (S.union (edges x) (edges y))
edges graph = S.empty

{-
    *** TODO ***

    Mulțimea nodurilor înspre care pleacă arce dinspre nodul curent.

    ATENȚIE! NU folosiți funcția edges definită mai sus, pentru că ar genera
    prea multe muchii inutile.
-}
outNeighbors :: Ord a => a -> AlgebraicGraph a -> S.Set a
outNeighbors node (Connect (Node x) y)
    | x == node = nodes y
    | otherwise = outNeighbors node y
outNeighbors node (Connect x y)
    | S.member node (nodes x) = S.union (nodes y) (outNeighbors node x)
    | otherwise = inNeighbors node y
outNeighbors node (Overlay x y) = S.union (outNeighbors node x) (outNeighbors node y)
outNeighbors node graph = S.empty


{-
    *** TODO ***

    Mulțimea nodurilor dinspre care pleacă arce înspre nodul curent.

    ATENȚIE! NU folosiți funcția edges definită mai sus, pentru că ar genera
    prea multe muchii inutile.
-}
inNeighbors :: Ord a => a -> AlgebraicGraph a -> S.Set a
inNeighbors node (Connect x (Node y))
    | y == node = nodes x
    | otherwise = inNeighbors node x
inNeighbors node (Connect x y)
    | S.member node (nodes y) = S.union (nodes x) (inNeighbors node y)
    | otherwise = inNeighbors node x
inNeighbors node (Overlay x y) = S.union (inNeighbors node x) (inNeighbors node y)
inNeighbors node graph = S.empty

{-
    *** TODO ***

    Întoarce graful rezultat prin eliminarea unui nod și a arcelor în care
    acesta este implicat. Dacă nodul nu există, se întoarce același graf.

    Hint: Definiți o funcție recursivă locală (de exemplu, în where),
    care să primească drept parametri doar entități variabile de la un apel
    recursiv la altul, astfel încât să nu copiați la fiecare apel parametrii
    nemodificați. De exemplu, parametrul node nu se modifică, în timp ce
    parametrul graph se modifică.
-}
removeNode :: Eq a => a -> AlgebraicGraph a -> AlgebraicGraph a
removeNode node = rmvNode
    where 
        rmvNode (Overlay x y)
            | rmvNode x == Empty && rmvNode y == Empty = Empty
            | rmvNode x == Empty = rmvNode y
            | rmvNode y == Empty = rmvNode x
            | otherwise = Overlay (rmvNode x) (rmvNode y)
        rmvNode (Connect x y)
            | rmvNode x == Empty && rmvNode y == Empty = Empty
            | rmvNode x == Empty = rmvNode y
            | rmvNode y == Empty = rmvNode x
            | otherwise = Connect (rmvNode x) (rmvNode y)
        rmvNode (Node x)
            | x == node = Empty
            | otherwise = Node x
        rmvNode Empty = Empty

{-
    *** TODO ***

    Divizează un nod în mai multe noduri, cu eliminarea nodului inițial.
    Arcele în care era implicat vechiul nod trebuie să devină valabile
    pentru noile noduri.
    
    Hint: Funcție recursivă locală, ca la removeNode.
-}
replaceNode :: Eq a => a -> AlgebraicGraph a -> AlgebraicGraph a -> AlgebraicGraph a
replaceNode old new Empty = Empty
replaceNode old new (Node x)
    | x == old = new
    | otherwise = Node x
replaceNode old new (Overlay x y) = Overlay (replaceNode old new x) (replaceNode old new y)
replaceNode old new (Connect x y) = Connect (replaceNode old new x) (replaceNode old new y)

splitNode :: Eq a
          => a                 -- nodul divizat
          -> [a]               -- nodurile cu care este înlocuit
          -> AlgebraicGraph a  -- graful existent
          -> AlgebraicGraph a  -- graful obținut
splitNode old news
    | length news > 1 || null news = replaceNode old (spltNode news Empty)  
    | otherwise = replaceNode old (Node (head news))
    where 
        spltNode [] node = node
        spltNode nodes Empty = spltNode (tail (tail nodes)) (Overlay (Node (head nodes)) (Node (head (tail nodes)))) 
        spltNode nodes node = spltNode (tail nodes) (Overlay (Node (head nodes)) node) 

{-
    *** TODO ***

    Îmbină mai multe noduri într-unul singur, pe baza unei proprietăți
    respectate de nodurile îmbinate, cu eliminarea acestora. Arcele în care
    erau implicate vechile noduri vor referi nodul nou.

    Hint: Funcție recursivă locală, ca la removeNode.
-}
mergeNodes :: Ord a => Eq a => (a -> Bool)       -- proprietatea îndeplinită de nodurile îmbinate
           -> a                 -- noul nod
           -> AlgebraicGraph a  -- graful existent
           -> AlgebraicGraph a  -- graful obținut
mergeNodes prop node graph = cleanDups $ foldl (\g n -> replaceNode n (Node node) g) graph (filter prop (S.toList (nodes graph)))
    where
        cleanDups Empty = Empty
        cleanDups (Node x) = Node x
        cleanDups (Overlay (Node x) (Node y))
            | x == node && y == node = Node node
            | otherwise = Overlay (Node x) (Node y)
        cleanDups (Overlay x (Node y))
            | y == node && S.member node (nodes x) = x
            | otherwise = Overlay x (Node y)
        cleanDups (Overlay (Node x) y)
            | x == node && S.member node (nodes y) = y
            | otherwise = Overlay (Node x) y
        cleanDups (Connect (Node x) (Node y))
            | x == node && y == node = Node node
            | otherwise = Connect (Node x) (Node y)
        cleanDups (Connect x (Node y))
            | y == node && S.member node (nodes x) = x
            | otherwise = Connect x (Node y)
        cleanDups (Connect (Node x) y)
            | x == node && S.member node (nodes y) = y
            | otherwise = Connect (Node x) y
        cleanDups graph = graph